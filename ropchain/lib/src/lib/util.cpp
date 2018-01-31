#include <algorithm>
#include <sstream>
#include <numeric>
#include "arch.h"
#include "util.h"
#include "config.h"

OptROP Util::toOptROP(const std::optional<ROPElem>& gadget) {
    if(gadget.has_value()) {
        return ROPChain(gadget.value());
    }
    return {};
}

std::vector<std::string> Util::split(std::string s, const char delim) {
    std::vector<std::string> ret;
    auto pos = std::string::npos;
    while((pos = s.find(delim)) != std::string::npos) {
        ret.push_back(s.substr(0, pos));
        s = s.substr(pos + 1);
    }
    if(s.length() && s.find(delim) == std::string::npos) {
        ret.push_back(s);
    }
    return ret;
}

std::string Util::join(const std::vector<std::string>& s, const std::string& separator) {
    std::ostringstream oss;
    if(!s.size()) {
        return "";
    }
    for(size_t i=0; i<s.size()-1; i++) {
        oss << s[i] << separator;
    }
    oss << s.back();
    return oss.str();
}

inline void ltrim(std::string& s, const std::string& delims) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](int ch) {
                return delims.find(ch) == std::string::npos;
                }));
}

inline void rtrim(std::string& s, const std::string& delims) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [&](int ch) {
                return delims.find(ch) == std::string::npos;
                }).base(), s.end());
}

void Util::trim(std::string& s, const std::string& delims) {
    ltrim(s, delims);
    rtrim(s, delims);
}

std::vector<RegType::Reg> *Util::toBits(const RegSet& s) {
	auto bits = new std::vector<RegType::Reg>();
	for(size_t i=0; i < s.size(); i++) {
		if(s.test(i)) {
			bits->push_back(i);
		}
	}
	return bits;
}

OptGadget Util::find(const Gadgets& gadgets, const RegSet& avl,
        const Mnem& mnem, const Operand op1) {
    return find(gadgets, avl, mnem, op1, {}, {});
}

OptGadget Util::find(const Gadgets& gadgets, const RegSet& avl,
        const Mnem& mnem, const Operand op1, const Operand op2) {
    return find(gadgets, avl, mnem, op1, op2, {});
}

OptGadget Util::find(const Gadgets& gadgets, const RegSet& avl,
        const Mnem& mnem, const std::optional<Operand> op1,
        const std::optional<Operand> op2,
        const std::optional<Operand> op3) {
    auto ops = std::vector<Operand>();
    if(op1.has_value()) {
        ops.push_back(op1.value());
    }
    if(op2.has_value()) {
        ops.push_back(op2.value());
    }
    if(op3.has_value()) {
        ops.push_back(op3.value());
    }
    const Insn insn(mnem, ops);
    for(const auto& gadget : gadgets) {
        const auto insns_g = gadget.insns;
        if(!insns_g.size()) {
            continue;
        }
        if(insn == insns_g[0] && gadget.isAvailable(avl)) {
            return gadget;
        }
    }
    return {};
}

RegType::Reg Util::findRegType(RegType::Reg reg) {
	switch(reg) {
	case RegType::rax: case RegType::eax:
	case RegType::ax: case RegType::ah: case RegType::al:
		return Arch::arch == Arch::X86 ? RegType::eax : RegType::rax;

	case RegType::rbx: case RegType::ebx:
	case RegType::bx: case RegType::bh: case RegType::bl:
		return Arch::arch == Arch::X86 ? RegType::ebx : RegType::rbx;

	case RegType::rcx: case RegType::ecx:
	case RegType::cx: case RegType::ch: case RegType::cl:
		return Arch::arch == Arch::X86 ? RegType::ecx : RegType::rcx;

	case RegType::rdx: case RegType::edx:
	case RegType::dx: case RegType::dh: case RegType::dl:
		return Arch::arch == Arch::X86 ? RegType::edx : RegType::rdx;

	case RegType::rdi: case RegType::edi:
	case RegType::di: case RegType::dil:
		return Arch::arch == Arch::X86 ? RegType::edi : RegType::rdi;

	case RegType::rsi: case RegType::esi:
	case RegType::si: case RegType::sil:
		return Arch::arch == Arch::X86 ? RegType::esi : RegType::rsi;

	case RegType::rbp: case RegType::ebp:
	case RegType::bp: case RegType::bpl:
		return Arch::arch == Arch::X86 ? RegType::ebp : RegType::rbp;

	case RegType::rsp: case RegType::esp:
	case RegType::sp: case RegType::spl:
		return Arch::arch == Arch::X86 ? RegType::esp : RegType::rsp;

	case RegType::r8: case RegType::r8d:
	case RegType::r8w: case RegType::r8b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r8;

	case RegType::r9: case RegType::r9d:
	case RegType::r9w: case RegType::r9b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r9;

	case RegType::r10: case RegType::r10d:
	case RegType::r10w: case RegType::r10b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r10;

	case RegType::r11: case RegType::r11d:
	case RegType::r11w: case RegType::r11b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r11;

	case RegType::r12: case RegType::r12d:
	case RegType::r12w: case RegType::r12b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r12;

	case RegType::r13: case RegType::r13d:
	case RegType::r13w: case RegType::r13b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r13;

	case RegType::r14: case RegType::r14d:
	case RegType::r14w: case RegType::r14b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r14;

	case RegType::r15: case RegType::r15d:
	case RegType::r15w: case RegType::r15b:
		return Arch::arch == Arch::X86 ? RegType::none : RegType::r15;

	default:
		return RegType::none;
	}
}

RegSet Util::listChangedRegs(const Insns& insns) {
	RegSet regs;
	for(const auto& insn : insns) {
		if(insn.ops.size() > 0) {
            if(const auto r = std::get_if<RegType::Reg>(&insn.ops[0])) {
                regs.set(*r);
            } else {
                ERR("Error: Unknown Register");
            }
		}
		if(insn.mnem == "xchg") {
            if(const auto r = std::get_if<RegType::Reg>(&insn.ops[0])) {
                regs.set(*r);
            } else {
                ERR("Error: Unknown Register");
            }
            if(const auto r = std::get_if<RegType::Reg>(&insn.ops[1])) {
                regs.set(*r);
            } else {
                ERR("Error: Unknown Register");
            }
		}
	}
	return regs;
}

size_t _calcUseStack(const Insn& insn) {
	const auto mnem = insn.mnem;
	const auto ops = insn.ops;
	if(mnem == "pop") {
		return Arch::word();
	} else if(mnem == "popad") {
		return Arch::word() * 7;
	} else if(mnem == "add") {
        if(const auto r = std::get_if<RegType::Reg>(&ops[0])) {
            if(*r == RegType::esp || *r == RegType::rsp) {
                return Arch::word() * std::get<uint64_t>(ops[1]);
            }
        }
	}
	return 0;
}

size_t Util::calcUseStack(const Insns& insns) {
    return std::accumulate(insns.begin(), insns.end(), 0,
            [](auto a, auto b) {return a + _calcUseStack(b);});
}

RegSet Util::map2Regs(const std::map<RegType::Reg, uint64_t>& m) {
    RegSet s;
    for(const auto& kv : m) {
        s.set(kv.first);
    }
    return s;
}

RegSet Util::allRegs() {
	RegSet s;
	if(Arch::arch == Arch::X86) {
        s.set(RegType::eax);
        s.set(RegType::ebx);
        s.set(RegType::ecx);
        s.set(RegType::edx);
        s.set(RegType::esi);
        s.set(RegType::edi);
        s.set(RegType::ebp);
		return s;
	}
	if(Arch::arch == Arch::AMD64) {
		s.set(RegType::rax);
		s.set(RegType::rbx);
		s.set(RegType::rcx);
		s.set(RegType::rdx);
		s.set(RegType::rdi);
		s.set(RegType::rsi);
		s.set(RegType::rbp);
		s.set(RegType::rsp);
		s.set(RegType::r8);
		s.set(RegType::r9);
		s.set(RegType::r10);
		s.set(RegType::r11);
		s.set(RegType::r12);
		s.set(RegType::r13);
		s.set(RegType::r14);
		s.set(RegType::r15);
		return s;
	}
	return RegSet();
}

std::string Util::pack(uint64_t v) {
	std::string ret;
	const size_t bytes = Arch::word();
	for(int i=bytes-1; i>=0; i--) {
		ret += (char)((v >> 8*i) & 0xff);
	}
	return ret;
}