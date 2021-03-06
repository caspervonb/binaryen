//===--- DWARFVisitor.cpp ---------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include "DWARFVisitor.h"
#include "llvm/ObjectYAML/DWARFYAML.h"

using namespace llvm;

template <typename T>
void DWARFYAML::VisitorImpl<T>::onVariableSizeValue(uint64_t U, unsigned Size) {
  switch (Size) {
  case 8:
    onValue((uint64_t)U);
    break;
  case 4:
    onValue((uint32_t)U);
    break;
  case 2:
    onValue((uint16_t)U);
    break;
  case 1:
    onValue((uint8_t)U);
    break;
  default:
    llvm_unreachable("Invalid integer write size.");
  }
}

static unsigned getOffsetSize(const DWARFYAML::Unit &Unit) {
  return Unit.Length.isDWARF64() ? 8 : 4;
}

static unsigned getRefSize(const DWARFYAML::Unit &Unit) {
  if (Unit.Version == 2)
    return Unit.AddrSize;
  return getOffsetSize(Unit);
}

template <typename T> void DWARFYAML::VisitorImpl<T>::traverseDebugInfo() {
  // XXX BINARYEN: Handle multiple linked compile units. Each one has its own
  // range of values, terminated by a zero. AbbrevStart refers to the start
  // index for the current unit, and AbbrevEnd to one past the last one
  // (which is the index of the 0 terminator).
  // TODO: This code appears to assume that abbreviation codes increment by 1
  // so that lookups are linear. In LLVM output that is true, but it might not
  // be in general.
  size_t AbbrevStart = 0, AbbrevEnd = -1;
  for (auto &Unit : DebugInfo.CompileUnits) {
    // Skip the 0 terminator.
    AbbrevEnd = AbbrevStart = AbbrevEnd + 1;
    while (AbbrevEnd < DebugInfo.AbbrevDecls.size() &&
           DebugInfo.AbbrevDecls[AbbrevEnd].Code) {
      AbbrevEnd++;
    }
    onStartCompileUnit(Unit);
    if (Unit.Entries.empty()) { // XXX BINARYEN
      continue;
    }
    auto FirstAbbrevCode = Unit.Entries[0].AbbrCode;
    for (auto &Entry : Unit.Entries) {
      onStartDIE(Unit, Entry);
      if (Entry.AbbrCode == 0u)
        continue;
      // XXX BINARYEN
      if (Entry.AbbrCode - FirstAbbrevCode + AbbrevStart >= AbbrevEnd) {
        errs() << "warning: invalid abbreviation code " << Entry.AbbrCode
               << " (range: " << FirstAbbrevCode << " : " << AbbrevStart
               << ".." << AbbrevEnd << ")\n";
        continue;
      }
      auto &Abbrev = DebugInfo.AbbrevDecls[Entry.AbbrCode - FirstAbbrevCode + AbbrevStart];
      auto FormVal = Entry.Values.begin();
      auto AbbrForm = Abbrev.Attributes.begin();
      for (;
           FormVal != Entry.Values.end() && AbbrForm != Abbrev.Attributes.end();
           ++FormVal, ++AbbrForm) {
        onForm(*AbbrForm, *FormVal);
        dwarf::Form Form = AbbrForm->Form;
        bool Indirect;
        do {
          Indirect = false;
          switch (Form) {
          case dwarf::DW_FORM_addr:
            onVariableSizeValue(FormVal->Value, Unit.AddrSize);
            break;
          case dwarf::DW_FORM_ref_addr:
            onVariableSizeValue(FormVal->Value, getRefSize(Unit));
            break;
          case dwarf::DW_FORM_exprloc:
          case dwarf::DW_FORM_block:
            onValue((uint64_t)FormVal->BlockData.size(), true);
            onValue(
                MemoryBufferRef(StringRef((const char *)&FormVal->BlockData[0],
                                          FormVal->BlockData.size()),
                                ""));
            break;
          case dwarf::DW_FORM_block1: {
            auto writeSize = FormVal->BlockData.size();
            onValue((uint8_t)writeSize);
            onValue(
                MemoryBufferRef(StringRef((const char *)&FormVal->BlockData[0],
                                          FormVal->BlockData.size()),
                                ""));
            break;
          }
          case dwarf::DW_FORM_block2: {
            auto writeSize = FormVal->BlockData.size();
            onValue((uint16_t)writeSize);
            onValue(
                MemoryBufferRef(StringRef((const char *)&FormVal->BlockData[0],
                                          FormVal->BlockData.size()),
                                ""));
            break;
          }
          case dwarf::DW_FORM_block4: {
            auto writeSize = FormVal->BlockData.size();
            onValue((uint32_t)writeSize);
            onValue(
                MemoryBufferRef(StringRef((const char *)&FormVal->BlockData[0],
                                          FormVal->BlockData.size()),
                                ""));
            break;
          }
          case dwarf::DW_FORM_data1:
          case dwarf::DW_FORM_ref1:
          case dwarf::DW_FORM_flag:
          case dwarf::DW_FORM_strx1:
          case dwarf::DW_FORM_addrx1:
            onValue((uint8_t)FormVal->Value);
            break;
          case dwarf::DW_FORM_data2:
          case dwarf::DW_FORM_ref2:
          case dwarf::DW_FORM_strx2:
          case dwarf::DW_FORM_addrx2:
            onValue((uint16_t)FormVal->Value);
            break;
          case dwarf::DW_FORM_data4:
          case dwarf::DW_FORM_ref4:
          case dwarf::DW_FORM_ref_sup4:
          case dwarf::DW_FORM_strx4:
          case dwarf::DW_FORM_addrx4:
            onValue((uint32_t)FormVal->Value);
            break;
          case dwarf::DW_FORM_data8:
          case dwarf::DW_FORM_ref8:
          case dwarf::DW_FORM_ref_sup8:
            onValue((uint64_t)FormVal->Value);
            break;
          case dwarf::DW_FORM_sdata:
            onValue((int64_t)FormVal->Value, true);
            break;
          case dwarf::DW_FORM_udata:
          case dwarf::DW_FORM_ref_udata:
            onValue((uint64_t)FormVal->Value, true);
            break;
          case dwarf::DW_FORM_string:
            onValue(FormVal->CStr);
            break;
          case dwarf::DW_FORM_indirect:
            onValue((uint64_t)FormVal->Value, true);
            Indirect = true;
            Form = static_cast<dwarf::Form>((uint64_t)FormVal->Value);
            ++FormVal;
            break;
          case dwarf::DW_FORM_strp:
          case dwarf::DW_FORM_sec_offset:
          case dwarf::DW_FORM_GNU_ref_alt:
          case dwarf::DW_FORM_GNU_strp_alt:
          case dwarf::DW_FORM_line_strp:
          case dwarf::DW_FORM_strp_sup:
            onVariableSizeValue(FormVal->Value, getOffsetSize(Unit));
            break;
          case dwarf::DW_FORM_ref_sig8:
            onValue((uint64_t)FormVal->Value);
            break;
          case dwarf::DW_FORM_GNU_addr_index:
          case dwarf::DW_FORM_GNU_str_index:
            onValue((uint64_t)FormVal->Value, true);
            break;
          default:
            break;
          }
        } while (Indirect);
      }
      onEndDIE(Unit, Entry);
    }
    onEndCompileUnit(Unit);
  }
}

// Explicitly instantiate the two template expansions.
template class DWARFYAML::VisitorImpl<DWARFYAML::Data>;
template class DWARFYAML::VisitorImpl<const DWARFYAML::Data>;
