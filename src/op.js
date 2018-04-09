/**
 * jsGB: Z80 core
 * Imran Nazar, May 2009
 * Notes: This is a GameBoy Z80, not a Z80. There are differences.
 * Bugs: If PC wraps at the top of memory, this will not be caught until the end of an instruction
 */
Z80 = {
    _r: {
        a: 0,
        b: 0,
        c: 0,
        d: 0,
        e: 0,
        h: 0,
        l: 0,
        f: 0,
        sp: 0,
        pc: 0,
        i: 0,
        r: 0,
        m: 0,
        ime: 0
    },

    _rsv: {
        a: 0,
        b: 0,
        c: 0,
        d: 0,
        e: 0,
        h: 0,
        l: 0,
        f: 0
    },

    _clock: {
        m: 0
    },

    _halt: 0,
    _stop: 0,

    reset: function() {
        reg8[A] = 0;
        reg8[B] = 0;
        reg8[C] = 0;
        reg8[D] = 0;
        reg8[E] = 0;
        reg8[H] = 0;
        reg8[L] = 0;
        reg8[F] = 0;
        sp = 0;
        pc = 0;
        Z80._r.i = 0;
        Z80._r.r = 0;
        m_cycle = 0;
        Z80._halt = 0;
        Z80._stop = 0;
        Z80._clock.m = 0;
        Z80._r.ime = 1;
        LOG.out('Z80', 'Reset.');
    },

    exec: function() {
        Z80._r.r = (Z80._r.r + 1) & 127;
        Z80._map[mem8->read_byte(pc++)]();
        pc &= 0xFFFF;
        Z80._clock.m += m_cycle;
    },

    _ops: {
        /*--- Load/store ---*/
        LDrr_bb: function() {
            reg8[B] = reg8[B];
            m_cycle = 1;
        },
        LDrr_bc: function() {
            reg8[B] = reg8[C];
            m_cycle = 1;
        },
        LDrr_bd: function() {
            reg8[B] = reg8[D];
            m_cycle = 1;
        },
        LDrr_be: function() {
            reg8[B] = reg8[E];
            m_cycle = 1;
        },
        LDrr_bh: function() {
            reg8[B] = reg8[H];
            m_cycle = 1;
        },
        LDrr_bl: function() {
            reg8[B] = reg8[L];
            m_cycle = 1;
        },
        LDrr_ba: function() {
            reg8[B] = reg8[A];
            m_cycle = 1;
        },
        LDrr_cb: function() {
            reg8[C] = reg8[B];
            m_cycle = 1;
        },
        LDrr_cc: function() {
            reg8[C] = reg8[C];
            m_cycle = 1;
        },
        LDrr_cd: function() {
            reg8[C] = reg8[D];
            m_cycle = 1;
        },
        LDrr_ce: function() {
            reg8[C] = reg8[E];
            m_cycle = 1;
        },
        LDrr_ch: function() {
            reg8[C] = reg8[H];
            m_cycle = 1;
        },
        LDrr_cl: function() {
            reg8[C] = reg8[L];
            m_cycle = 1;
        },
        LDrr_ca: function() {
            reg8[C] = reg8[A];
            m_cycle = 1;
        },
        LDrr_db: function() {
            reg8[D] = reg8[B];
            m_cycle = 1;
        },
        LDrr_dc: function() {
            reg8[D] = reg8[C];
            m_cycle = 1;
        },
        LDrr_dd: function() {
            reg8[D] = reg8[D];
            m_cycle = 1;
        },
        LDrr_de: function() {
            reg8[D] = reg8[E];
            m_cycle = 1;
        },
        LDrr_dh: function() {
            reg8[D] = reg8[H];
            m_cycle = 1;
        },
        LDrr_dl: function() {
            reg8[D] = reg8[L];
            m_cycle = 1;
        },
        LDrr_da: function() {
            reg8[D] = reg8[A];
            m_cycle = 1;
        },
        LDrr_eb: function() {
            reg8[E] = reg8[B];
            m_cycle = 1;
        },
        LDrr_ec: function() {
            reg8[E] = reg8[C];
            m_cycle = 1;
        },
        LDrr_ed: function() {
            reg8[E] = reg8[D];
            m_cycle = 1;
        },
        LDrr_ee: function() {
            reg8[E] = reg8[E];
            m_cycle = 1;
        },
        LDrr_eh: function() {
            reg8[E] = reg8[H];
            m_cycle = 1;
        },
        LDrr_el: function() {
            reg8[E] = reg8[L];
            m_cycle = 1;
        },
        LDrr_ea: function() {
            reg8[E] = reg8[A];
            m_cycle = 1;
        },
        LDrr_hb: function() {
            reg8[H] = reg8[B];
            m_cycle = 1;
        },
        LDrr_hc: function() {
            reg8[H] = reg8[C];
            m_cycle = 1;
        },
        LDrr_hd: function() {
            reg8[H] = reg8[D];
            m_cycle = 1;
        },
        LDrr_he: function() {
            reg8[H] = reg8[E];
            m_cycle = 1;
        },
        LDrr_hh: function() {
            reg8[H] = reg8[H];
            m_cycle = 1;
        },
        LDrr_hl: function() {
            reg8[H] = reg8[L];
            m_cycle = 1;
        },
        LDrr_ha: function() {
            reg8[H] = reg8[A];
            m_cycle = 1;
        },
        LDrr_lb: function() {
            reg8[L] = reg8[B];
            m_cycle = 1;
        },
        LDrr_lc: function() {
            reg8[L] = reg8[C];
            m_cycle = 1;
        },
        LDrr_ld: function() {
            reg8[L] = reg8[D];
            m_cycle = 1;
        },
        LDrr_le: function() {
            reg8[L] = reg8[E];
            m_cycle = 1;
        },
        LDrr_lh: function() {
            reg8[L] = reg8[H];
            m_cycle = 1;
        },
        LDrr_ll: function() {
            reg8[L] = reg8[L];
            m_cycle = 1;
        },
        LDrr_la: function() {
            reg8[L] = reg8[A];
            m_cycle = 1;
        },
        LDrr_ab: function() {
            reg8[A] = reg8[B];
            m_cycle = 1;
        },
        LDrr_ac: function() {
            reg8[A] = reg8[C];
            m_cycle = 1;
        },
        LDrr_ad: function() {
            reg8[A] = reg8[D];
            m_cycle = 1;
        },
        LDrr_ae: function() {
            reg8[A] = reg8[E];
            m_cycle = 1;
        },
        LDrr_ah: function() {
            reg8[A] = reg8[H];
            m_cycle = 1;
        },
        LDrr_al: function() {
            reg8[A] = reg8[L];
            m_cycle = 1;
        },
        LDrr_aa: function() {
            reg8[A] = reg8[A];
            m_cycle = 1;
        },

        LDrHLm_b: function() {
            reg8[B] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },
        LDrHLm_c: function() {
            reg8[C] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },
        LDrHLm_d: function() {
            reg8[D] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },
        LDrHLm_e: function() {
            reg8[E] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },
        LDrHLm_h: function() {
            reg8[H] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },
        LDrHLm_l: function() {
            reg8[L] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },
        LDrHLm_a: function() {
            reg8[A] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            m_cycle = 2;
        },

        LDHLmr_b: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[B]);
            m_cycle = 2;
        },
        LDHLmr_c: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[C]);
            m_cycle = 2;
        },
        LDHLmr_d: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[D]);
            m_cycle = 2;
        },
        LDHLmr_e: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[E]);
            m_cycle = 2;
        },
        LDHLmr_h: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[H]);
            m_cycle = 2;
        },
        LDHLmr_l: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[L]);
            m_cycle = 2;
        },
        LDHLmr_a: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[A]);
            m_cycle = 2;
        },

        LDrn_b: function() {
            reg8[B] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },
        LDrn_c: function() {
            reg8[C] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },
        LDrn_d: function() {
            reg8[D] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },
        LDrn_e: function() {
            reg8[E] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },
        LDrn_h: function() {
            reg8[H] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },
        LDrn_l: function() {
            reg8[L] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },
        LDrn_a: function() {
            reg8[A] = mem8->read_byte(pc);
            pc++;
            m_cycle = 2;
        },

        LDHLmn: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], mem8->read_byte(pc));
            pc++;
            m_cycle = 3;
        },

        LDBCmA: function() {
            mem8->write_byte((reg8[B] << 8) + reg8[C], reg8[A]);
            m_cycle = 2;
        },
        LDDEmA: function() {
            mem8->write_byte((reg8[D] << 8) + reg8[E], reg8[A]);
            m_cycle = 2;
        },

        LDmmA: function() {
            mem8->write_byte(mem8->read_word(pc), reg8[A]);
            pc += 2;
            m_cycle = 4;
        },

        LDABCm: function() {
            reg8[A] = mem8->read_byte((reg8[B] << 8) + reg8[C]);
            m_cycle = 2;
        },
        LDADEm: function() {
            reg8[A] = mem8->read_byte((reg8[D] << 8) + reg8[E]);
            m_cycle = 2;
        },

        LDAmm: function() {
            reg8[A] = mem8->read_byte(mem8->read_word(pc));
            pc += 2;
            m_cycle = 4;
        },

        LDBCnn: function() {
            reg8[C] = mem8->read_byte(pc);
            reg8[B] = mem8->read_byte(pc + 1);
            pc += 2;
            m_cycle = 3;
        },
        LDDEnn: function() {
            reg8[E] = mem8->read_byte(pc);
            reg8[D] = mem8->read_byte(pc + 1);
            pc += 2;
            m_cycle = 3;
        },
        LDHLnn: function() {
            reg8[L] = mem8->read_byte(pc);
            reg8[H] = mem8->read_byte(pc + 1);
            pc += 2;
            m_cycle = 3;
        },
        LDSPnn: function() {
            sp = mem8->read_word(pc);
            pc += 2;
            m_cycle = 3;
        },

        LDHLmm: function() {
            int i = mem8->read_word(pc);
            pc += 2;
            reg8[L] = mem8->read_byte(i);
            reg8[H] = mem8->read_byte(i + 1);
            m_cycle = 5;
        },
        LDmmHL: function() {
            int i = mem8->read_word(pc);
            pc += 2;
            mem8->write_word(i, (reg8[H] << 8) + reg8[L]);
            m_cycle = 5;
        },

        LDHLIA: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[A]);
            reg8[L] = (reg8[L] + 1) & 0xFF;
            if (!reg8[L]) reg8[H] = (reg8[H] + 1) & 0xFF;
            m_cycle = 2;
        },
        LDAHLI: function() {
            reg8[A] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[L] = (reg8[L] + 1) & 0xFF;
            if (!reg8[L]) reg8[H] = (reg8[H] + 1) & 0xFF;
            m_cycle = 2;
        },

        LDHLDA: function() {
            mem8->write_byte((reg8[H] << 8) + reg8[L], reg8[A]);
            reg8[L] = (reg8[L] - 1) & 0xFF;
            if (reg8[L] == 0xFF) reg8[H] = (reg8[H] - 1) & 0xFF;
            m_cycle = 2;
        },
        LDAHLD: function() {
            reg8[A] = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[L] = (reg8[L] - 1) & 0xFF;
            if (reg8[L] == 0xFF) reg8[H] = (reg8[H] - 1) & 0xFF;
            m_cycle = 2;
        },

        LDAIOn: function() {
            reg8[A] = mem8->read_byte(0xFF00 + mem8->read_byte(pc));
            pc++;
            m_cycle = 3;
        },
        LDIOnA: function() {
            mem8->write_byte(0xFF00 + mem8->read_byte(pc), reg8[A]);
            pc++;
            m_cycle = 3;
        },
        LDAIOC: function() {
            reg8[A] = mem8->read_byte(0xFF00 + reg8[C]);
            m_cycle = 2;
        },
        LDIOCA: function() {
            mem8->write_byte(0xFF00 + reg8[C], reg8[A]);
            m_cycle = 2;
        },

        LDHLSPn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            i += sp;
            reg8[H] = (i >> 8) & 0xFF;
            reg8[L] = i & 0xFF;
            m_cycle = 3;
        },

        SWAPr_b: function() {
            int tr = reg8[B];
            reg8[B] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[B] ? 0 : 0x80;
            m_cycle = 1;
        },
        SWAPr_c: function() {
            int tr = reg8[C];
            reg8[C] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[C] ? 0 : 0x80;
            m_cycle = 1;
        },
        SWAPr_d: function() {
            int tr = reg8[D];
            reg8[D] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[D] ? 0 : 0x80;
            m_cycle = 1;
        },
        SWAPr_e: function() {
            int tr = reg8[E];
            reg8[E] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[E] ? 0 : 0x80;
            m_cycle = 1;
        },
        SWAPr_h: function() {
            int tr = reg8[H];
            reg8[H] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[H] ? 0 : 0x80;
            m_cycle = 1;
        },
        SWAPr_l: function() {
            int tr = reg8[L];
            reg8[L] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[L] ? 0 : 0x80;
            m_cycle = 1;
        },
        SWAPr_a: function() {
            int tr = reg8[A];
            reg8[A] = ((tr & 0xF) << 4) | ((tr & 0xF0) >> 4);
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },

        /*--- Data processing ---*/
        ADDr_b: function() {
            int a = reg8[A];
            reg8[A] += reg8[B];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[B] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDr_c: function() {
            int a = reg8[A];
            reg8[A] += reg8[C];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[C] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDr_d: function() {
            int a = reg8[A];
            reg8[A] += reg8[D];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[D] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDr_e: function() {
            int a = reg8[A];
            reg8[A] += reg8[E];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[E] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDr_h: function() {
            int a = reg8[A];
            reg8[A] += reg8[H];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[H] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDr_l: function() {
            int a = reg8[A];
            reg8[A] += reg8[L];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[L] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDr_a: function() {
            int a = reg8[A];
            reg8[A] += reg8[A];
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[A] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADDHL: function() {
            int a = reg8[A];
            int m = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] += m;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ a ^ m) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },
        ADDn: function() {
            int a = reg8[A];
            int m = mem8->read_byte(pc);
            reg8[A] += m;
            pc++;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ a ^ m) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },
        ADDHLBC: function() {
            int hl = (reg8[H] << 8) + reg8[L];
            hl += (reg8[B] << 8) + reg8[C];
            if (hl > 0xFFFF) reg8[F] |= 0x10;
            else reg8[F] &= 0xEF;
            reg8[H] = (hl >> 8) & 0xFF;
            reg8[L] = hl & 0xFF;
            m_cycle = 3;
        },
        ADDHLDE: function() {
            int hl = (reg8[H] << 8) + reg8[L];
            hl += (reg8[D] << 8) + reg8[E];
            if (hl > 0xFFFF) reg8[F] |= 0x10;
            else reg8[F] &= 0xEF;
            reg8[H] = (hl >> 8) & 0xFF;
            reg8[L] = hl & 0xFF;
            m_cycle = 3;
        },
        ADDHLHL: function() {
            int hl = (reg8[H] << 8) + reg8[L];
            hl += (reg8[H] << 8) + reg8[L];
            if (hl > 0xFFFF) reg8[F] |= 0x10;
            else reg8[F] &= 0xEF;
            reg8[H] = (hl >> 8) & 0xFF;
            reg8[L] = hl & 0xFF;
            m_cycle = 3;
        },
        ADDHLSP: function() {
            int hl = (reg8[H] << 8) + reg8[L];
            hl += sp;
            if (hl > 0xFFFF) reg8[F] |= 0x10;
            else reg8[F] &= 0xEF;
            reg8[H] = (hl >> 8) & 0xFF;
            reg8[L] = hl & 0xFF;
            m_cycle = 3;
        },
        ADDSPn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            sp += i;
            m_cycle = 4;
        },

        ADCr_b: function() {
            int a = reg8[A];
            reg8[A] += reg8[B];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[B] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCr_c: function() {
            int a = reg8[A];
            reg8[A] += reg8[C];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[C] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCr_d: function() {
            int a = reg8[A];
            reg8[A] += reg8[D];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[D] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCr_e: function() {
            int a = reg8[A];
            reg8[A] += reg8[E];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[E] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCr_h: function() {
            int a = reg8[A];
            reg8[A] += reg8[H];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[H] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCr_l: function() {
            int a = reg8[A];
            reg8[A] += reg8[L];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[L] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCr_a: function() {
            int a = reg8[A];
            reg8[A] += reg8[A];
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[A] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        ADCHL: function() {
            int a = reg8[A];
            int m = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] += m;
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ m ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },
        ADCn: function() {
            int a = reg8[A];
            int m = mem8->read_byte(pc);
            reg8[A] += m;
            pc++;
            reg8[A] += (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] > 0xFF) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ m ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },

        SUBr_b: function() {
            int a = reg8[A];
            reg8[A] -= reg8[B];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[B] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBr_c: function() {
            int a = reg8[A];
            reg8[A] -= reg8[C];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[C] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBr_d: function() {
            int a = reg8[A];
            reg8[A] -= reg8[D];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[D] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBr_e: function() {
            int a = reg8[A];
            reg8[A] -= reg8[E];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[E] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBr_h: function() {
            int a = reg8[A];
            reg8[A] -= reg8[H];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[H] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBr_l: function() {
            int a = reg8[A];
            reg8[A] -= reg8[L];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[L] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBr_a: function() {
            int a = reg8[A];
            reg8[A] -= reg8[A];
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[A] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SUBHL: function() {
            int a = reg8[A];
            int m = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] -= m;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ m ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },
        SUBn: function() {
            int a = reg8[A];
            int m = mem8->read_byte(pc);
            reg8[A] -= m;
            pc++;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ m ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },

        SBCr_b: function() {
            int a = reg8[A];
            reg8[A] -= reg8[B];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[B] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCr_c: function() {
            int a = reg8[A];
            reg8[A] -= reg8[C];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[C] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCr_d: function() {
            int a = reg8[A];
            reg8[A] -= reg8[D];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[D] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCr_e: function() {
            int a = reg8[A];
            reg8[A] -= reg8[E];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[E] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCr_h: function() {
            int a = reg8[A];
            reg8[A] -= reg8[H];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[H] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCr_l: function() {
            int a = reg8[A];
            reg8[A] -= reg8[L];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[L] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCr_a: function() {
            int a = reg8[A];
            reg8[A] -= reg8[A];
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[A] ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        SBCHL: function() {
            int a = reg8[A];
            int m = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] -= m;
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ m ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },
        SBCn: function() {
            int a = reg8[A];
            int m = mem8->read_byte(pc);
            reg8[A] -= m;
            pc++;
            reg8[A] -= (reg8[F] & 0x10) ? 1 : 0;
            reg8[F] = (reg8[A] < 0) ? 0x50 : 0x40;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            if ((reg8[A] ^ m ^ a) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },

        CPr_b: function() {
            int i = reg8[A];
            i -= reg8[B];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[B] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPr_c: function() {
            int i = reg8[A];
            i -= reg8[C];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[C] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPr_d: function() {
            int i = reg8[A];
            i -= reg8[D];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[D] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPr_e: function() {
            int i = reg8[A];
            i -= reg8[E];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[E] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPr_h: function() {
            int i = reg8[A];
            i -= reg8[H];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[H] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPr_l: function() {
            int i = reg8[A];
            i -= reg8[L];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[L] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPr_a: function() {
            int i = reg8[A];
            i -= reg8[A];
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ reg8[A] ^ i) & 0x10) reg8[F] |= 0x20;
            m_cycle = 1;
        },
        CPHL: function() {
            int i = reg8[A];
            int m = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i -= m;
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ i ^ m) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },
        CPn: function() {
            int i = reg8[A];
            int m = mem8->read_byte(pc);
            i -= m;
            pc++;
            reg8[F] = (i < 0) ? 0x50 : 0x40;
            i &= 0xFF;
            if (!i) reg8[F] |= 0x80;
            if ((reg8[A] ^ i ^ m) & 0x10) reg8[F] |= 0x20;
            m_cycle = 2;
        },

        DAA: function() {
            int a = reg8[A];
            if ((reg8[F] & 0x20) || ((reg8[A] & 15) > 9)) reg8[A] += 6;
            reg8[F] &= 0xEF;
            if ((reg8[F] & 0x20) || (a > 0x99)) {
                reg8[A] += 0x60;
                reg8[F] |= 0x10;
            }
            m_cycle = 1;
        },

        ANDr_b: function() {
            reg8[A] &= reg8[B];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDr_c: function() {
            reg8[A] &= reg8[C];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDr_d: function() {
            reg8[A] &= reg8[D];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDr_e: function() {
            reg8[A] &= reg8[E];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDr_h: function() {
            reg8[A] &= reg8[H];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDr_l: function() {
            reg8[A] &= reg8[L];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDr_a: function() {
            reg8[A] &= reg8[A];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ANDHL: function() {
            reg8[A] &= mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 2;
        },
        ANDn: function() {
            reg8[A] &= mem8->read_byte(pc);
            pc++;
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 2;
        },

        ORr_b: function() {
            reg8[A] |= reg8[B];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORr_c: function() {
            reg8[A] |= reg8[C];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORr_d: function() {
            reg8[A] |= reg8[D];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORr_e: function() {
            reg8[A] |= reg8[E];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORr_h: function() {
            reg8[A] |= reg8[H];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORr_l: function() {
            reg8[A] |= reg8[L];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORr_a: function() {
            reg8[A] |= reg8[A];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        ORHL: function() {
            reg8[A] |= mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 2;
        },
        ORn: function() {
            reg8[A] |= mem8->read_byte(pc);
            pc++;
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 2;
        },

        XORr_b: function() {
            reg8[A] ^= reg8[B];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORr_c: function() {
            reg8[A] ^= reg8[C];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORr_d: function() {
            reg8[A] ^= reg8[D];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORr_e: function() {
            reg8[A] ^= reg8[E];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORr_h: function() {
            reg8[A] ^= reg8[H];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORr_l: function() {
            reg8[A] ^= reg8[L];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORr_a: function() {
            reg8[A] ^= reg8[A];
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        XORHL: function() {
            reg8[A] ^= mem8->read_byte((reg8[H] << 8) + reg8[L]);
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 2;
        },
        XORn: function() {
            reg8[A] ^= mem8->read_byte(pc);
            pc++;
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 2;
        },

        INCr_b: function() {
            reg8[B]++;
            reg8[B] &= 0xFF;
            reg8[F] = reg8[B] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCr_c: function() {
            reg8[C]++;
            reg8[C] &= 0xFF;
            reg8[F] = reg8[C] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCr_d: function() {
            reg8[D]++;
            reg8[D] &= 0xFF;
            reg8[F] = reg8[D] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCr_e: function() {
            reg8[E]++;
            reg8[E] &= 0xFF;
            reg8[F] = reg8[E] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCr_h: function() {
            reg8[H]++;
            reg8[H] &= 0xFF;
            reg8[F] = reg8[H] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCr_l: function() {
            reg8[L]++;
            reg8[L] &= 0xFF;
            reg8[F] = reg8[L] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCr_a: function() {
            reg8[A]++;
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        INCHLm: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]) + 1;
            i &= 0xFF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            reg8[F] = i ? 0 : 0x80;
            m_cycle = 3;
        },

        DECr_b: function() {
            reg8[B]--;
            reg8[B] &= 0xFF;
            reg8[F] = reg8[B] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECr_c: function() {
            reg8[C]--;
            reg8[C] &= 0xFF;
            reg8[F] = reg8[C] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECr_d: function() {
            reg8[D]--;
            reg8[D] &= 0xFF;
            reg8[F] = reg8[D] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECr_e: function() {
            reg8[E]--;
            reg8[E] &= 0xFF;
            reg8[F] = reg8[E] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECr_h: function() {
            reg8[H]--;
            reg8[H] &= 0xFF;
            reg8[F] = reg8[H] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECr_l: function() {
            reg8[L]--;
            reg8[L] &= 0xFF;
            reg8[F] = reg8[L] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECr_a: function() {
            reg8[A]--;
            reg8[A] &= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        DECHLm: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]) - 1;
            i &= 0xFF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            reg8[F] = i ? 0 : 0x80;
            m_cycle = 3;
        },

        INCBC: function() {
            reg8[C] = (reg8[C] + 1) & 0xFF;
            if (!reg8[C]) reg8[B] = (reg8[B] + 1) & 0xFF;
            m_cycle = 1;
        },
        INCDE: function() {
            reg8[E] = (reg8[E] + 1) & 0xFF;
            if (!reg8[E]) reg8[D] = (reg8[D] + 1) & 0xFF;
            m_cycle = 1;
        },
        INCHL: function() {
            reg8[L] = (reg8[L] + 1) & 0xFF;
            if (!reg8[L]) reg8[H] = (reg8[H] + 1) & 0xFF;
            m_cycle = 1;
        },
        INCSP: function() {
            sp = (sp + 1) & 0xFFFF;
            m_cycle = 1;
        },

        DECBC: function() {
            reg8[C] = (reg8[C] - 1) & 0xFF;
            if (reg8[C] == 0xFF) reg8[B] = (reg8[B] - 1) & 0xFF;
            m_cycle = 1;
        },
        DECDE: function() {
            reg8[E] = (reg8[E] - 1) & 0xFF;
            if (reg8[E] == 0xFF) reg8[D] = (reg8[D] - 1) & 0xFF;
            m_cycle = 1;
        },
        DECHL: function() {
            reg8[L] = (reg8[L] - 1) & 0xFF;
            if (reg8[L] == 0xFF) reg8[H] = (reg8[H] - 1) & 0xFF;
            m_cycle = 1;
        },
        DECSP: function() {
            sp = (sp - 1) & 0xFFFF;
            m_cycle = 1;
        },

        /*--- Bit manipulation ---*/
        BIT0b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x01) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT0m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x01) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES0b: function() {
            reg8[B] &= 0xFE;
            m_cycle = 2;
        },
        RES0c: function() {
            reg8[C] &= 0xFE;
            m_cycle = 2;
        },
        RES0d: function() {
            reg8[D] &= 0xFE;
            m_cycle = 2;
        },
        RES0e: function() {
            reg8[E] &= 0xFE;
            m_cycle = 2;
        },
        RES0h: function() {
            reg8[H] &= 0xFE;
            m_cycle = 2;
        },
        RES0l: function() {
            reg8[L] &= 0xFE;
            m_cycle = 2;
        },
        RES0a: function() {
            reg8[A] &= 0xFE;
            m_cycle = 2;
        },
        RES0m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xFE;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET0b: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0c: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0d: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0e: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0h: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0l: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0a: function() {
            reg8[B] |= 0x01;
            m_cycle = 2;
        },
        SET0m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x01;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT1b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x02) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT1m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x02) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES1b: function() {
            reg8[B] &= 0xFD;
            m_cycle = 2;
        },
        RES1c: function() {
            reg8[C] &= 0xFD;
            m_cycle = 2;
        },
        RES1d: function() {
            reg8[D] &= 0xFD;
            m_cycle = 2;
        },
        RES1e: function() {
            reg8[E] &= 0xFD;
            m_cycle = 2;
        },
        RES1h: function() {
            reg8[H] &= 0xFD;
            m_cycle = 2;
        },
        RES1l: function() {
            reg8[L] &= 0xFD;
            m_cycle = 2;
        },
        RES1a: function() {
            reg8[A] &= 0xFD;
            m_cycle = 2;
        },
        RES1m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xFD;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET1b: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1c: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1d: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1e: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1h: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1l: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1a: function() {
            reg8[B] |= 0x02;
            m_cycle = 2;
        },
        SET1m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x02;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT2b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x04) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT2m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x04) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES2b: function() {
            reg8[B] &= 0xFB;
            m_cycle = 2;
        },
        RES2c: function() {
            reg8[C] &= 0xFB;
            m_cycle = 2;
        },
        RES2d: function() {
            reg8[D] &= 0xFB;
            m_cycle = 2;
        },
        RES2e: function() {
            reg8[E] &= 0xFB;
            m_cycle = 2;
        },
        RES2h: function() {
            reg8[H] &= 0xFB;
            m_cycle = 2;
        },
        RES2l: function() {
            reg8[L] &= 0xFB;
            m_cycle = 2;
        },
        RES2a: function() {
            reg8[A] &= 0xFB;
            m_cycle = 2;
        },
        RES2m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xFB;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET2b: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2c: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2d: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2e: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2h: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2l: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2a: function() {
            reg8[B] |= 0x04;
            m_cycle = 2;
        },
        SET2m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x04;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT3b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x08) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT3m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x08) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES3b: function() {
            reg8[B] &= 0xF7;
            m_cycle = 2;
        },
        RES3c: function() {
            reg8[C] &= 0xF7;
            m_cycle = 2;
        },
        RES3d: function() {
            reg8[D] &= 0xF7;
            m_cycle = 2;
        },
        RES3e: function() {
            reg8[E] &= 0xF7;
            m_cycle = 2;
        },
        RES3h: function() {
            reg8[H] &= 0xF7;
            m_cycle = 2;
        },
        RES3l: function() {
            reg8[L] &= 0xF7;
            m_cycle = 2;
        },
        RES3a: function() {
            reg8[A] &= 0xF7;
            m_cycle = 2;
        },
        RES3m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xF7;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET3b: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3c: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3d: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3e: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3h: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3l: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3a: function() {
            reg8[B] |= 0x08;
            m_cycle = 2;
        },
        SET3m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x08;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT4b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x10) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT4m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x10) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES4b: function() {
            reg8[B] &= 0xEF;
            m_cycle = 2;
        },
        RES4c: function() {
            reg8[C] &= 0xEF;
            m_cycle = 2;
        },
        RES4d: function() {
            reg8[D] &= 0xEF;
            m_cycle = 2;
        },
        RES4e: function() {
            reg8[E] &= 0xEF;
            m_cycle = 2;
        },
        RES4h: function() {
            reg8[H] &= 0xEF;
            m_cycle = 2;
        },
        RES4l: function() {
            reg8[L] &= 0xEF;
            m_cycle = 2;
        },
        RES4a: function() {
            reg8[A] &= 0xEF;
            m_cycle = 2;
        },
        RES4m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xEF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET4b: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4c: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4d: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4e: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4h: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4l: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4a: function() {
            reg8[B] |= 0x10;
            m_cycle = 2;
        },
        SET4m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x10;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT5b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x20) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT5m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x20) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES5b: function() {
            reg8[B] &= 0xDF;
            m_cycle = 2;
        },
        RES5c: function() {
            reg8[C] &= 0xDF;
            m_cycle = 2;
        },
        RES5d: function() {
            reg8[D] &= 0xDF;
            m_cycle = 2;
        },
        RES5e: function() {
            reg8[E] &= 0xDF;
            m_cycle = 2;
        },
        RES5h: function() {
            reg8[H] &= 0xDF;
            m_cycle = 2;
        },
        RES5l: function() {
            reg8[L] &= 0xDF;
            m_cycle = 2;
        },
        RES5a: function() {
            reg8[A] &= 0xDF;
            m_cycle = 2;
        },
        RES5m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xDF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET5b: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5c: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5d: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5e: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5h: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5l: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5a: function() {
            reg8[B] |= 0x20;
            m_cycle = 2;
        },
        SET5m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x20;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT6b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x40) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT6m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x40) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES6b: function() {
            reg8[B] &= 0xBF;
            m_cycle = 2;
        },
        RES6c: function() {
            reg8[C] &= 0xBF;
            m_cycle = 2;
        },
        RES6d: function() {
            reg8[D] &= 0xBF;
            m_cycle = 2;
        },
        RES6e: function() {
            reg8[E] &= 0xBF;
            m_cycle = 2;
        },
        RES6h: function() {
            reg8[H] &= 0xBF;
            m_cycle = 2;
        },
        RES6l: function() {
            reg8[L] &= 0xBF;
            m_cycle = 2;
        },
        RES6a: function() {
            reg8[A] &= 0xBF;
            m_cycle = 2;
        },
        RES6m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0xBF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET6b: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6c: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6d: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6e: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6h: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6l: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6a: function() {
            reg8[B] |= 0x40;
            m_cycle = 2;
        },
        SET6m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x40;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        BIT7b: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[B] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7c: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[C] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7d: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[D] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7e: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[E] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7h: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[H] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7l: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[L] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7a: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (reg8[A] & 0x80) ? 0 : 0x80;
            m_cycle = 2;
        },
        BIT7m: function() {
            reg8[F] &= 0x1F;
            reg8[F] |= 0x20;
            reg8[F] = (mem8->read_byte((reg8[H] << 8) + reg8[L]) & 0x80) ? 0 : 0x80;
            m_cycle = 3;
        },

        RES7b: function() {
            reg8[B] &= 0x7F;
            m_cycle = 2;
        },
        RES7c: function() {
            reg8[C] &= 0x7F;
            m_cycle = 2;
        },
        RES7d: function() {
            reg8[D] &= 0x7F;
            m_cycle = 2;
        },
        RES7e: function() {
            reg8[E] &= 0x7F;
            m_cycle = 2;
        },
        RES7h: function() {
            reg8[H] &= 0x7F;
            m_cycle = 2;
        },
        RES7l: function() {
            reg8[L] &= 0x7F;
            m_cycle = 2;
        },
        RES7a: function() {
            reg8[A] &= 0x7F;
            m_cycle = 2;
        },
        RES7m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i &= 0x7F;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        SET7b: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7c: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7d: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7e: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7h: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7l: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7a: function() {
            reg8[B] |= 0x80;
            m_cycle = 2;
        },
        SET7m: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            i |= 0x80;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            m_cycle = 4;
        },

        RLA: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[A] & 0x80 ? 0x10 : 0;
            reg8[A] = (reg8[A] << 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 1;
        },
        RLCA: function() {
            int ci = reg8[A] & 0x80 ? 1 : 0;
            int co = reg8[A] & 0x80 ? 0x10 : 0;
            reg8[A] = (reg8[A] << 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 1;
        },
        RRA: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[A] & 1 ? 0x10 : 0;
            reg8[A] = (reg8[A] >> 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 1;
        },
        RRCA: function() {
            int ci = reg8[A] & 1 ? 0x80 : 0;
            int co = reg8[A] & 1 ? 0x10 : 0;
            reg8[A] = (reg8[A] >> 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 1;
        },

        RLr_b: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[B] & 0x80 ? 0x10 : 0;
            reg8[B] = (reg8[B] << 1) + ci;
            reg8[B] &= 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLr_c: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[C] & 0x80 ? 0x10 : 0;
            reg8[C] = (reg8[C] << 1) + ci;
            reg8[C] &= 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLr_d: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[D] & 0x80 ? 0x10 : 0;
            reg8[D] = (reg8[D] << 1) + ci;
            reg8[D] &= 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLr_e: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[E] & 0x80 ? 0x10 : 0;
            reg8[E] = (reg8[E] << 1) + ci;
            reg8[E] &= 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLr_h: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[H] & 0x80 ? 0x10 : 0;
            reg8[H] = (reg8[H] << 1) + ci;
            reg8[H] &= 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLr_l: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[L] & 0x80 ? 0x10 : 0;
            reg8[L] = (reg8[L] << 1) + ci;
            reg8[L] &= 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLr_a: function() {
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = reg8[A] & 0x80 ? 0x10 : 0;
            reg8[A] = (reg8[A] << 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLHL: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            int ci = reg8[F] & 0x10 ? 1 : 0;
            int co = i & 0x80 ? 0x10 : 0;
            i = (i << 1) + ci;
            i &= 0xFF;
            reg8[F] = (i) ? 0 : 0x80;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 4;
        },

        RLCr_b: function() {
            int ci = reg8[B] & 0x80 ? 1 : 0;
            int co = reg8[B] & 0x80 ? 0x10 : 0;
            reg8[B] = (reg8[B] << 1) + ci;
            reg8[B] &= 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCr_c: function() {
            int ci = reg8[C] & 0x80 ? 1 : 0;
            int co = reg8[C] & 0x80 ? 0x10 : 0;
            reg8[C] = (reg8[C] << 1) + ci;
            reg8[C] &= 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCr_d: function() {
            int ci = reg8[D] & 0x80 ? 1 : 0;
            int co = reg8[D] & 0x80 ? 0x10 : 0;
            reg8[D] = (reg8[D] << 1) + ci;
            reg8[D] &= 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCr_e: function() {
            int ci = reg8[E] & 0x80 ? 1 : 0;
            int co = reg8[E] & 0x80 ? 0x10 : 0;
            reg8[E] = (reg8[E] << 1) + ci;
            reg8[E] &= 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCr_h: function() {
            int ci = reg8[H] & 0x80 ? 1 : 0;
            int co = reg8[H] & 0x80 ? 0x10 : 0;
            reg8[H] = (reg8[H] << 1) + ci;
            reg8[H] &= 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCr_l: function() {
            int ci = reg8[L] & 0x80 ? 1 : 0;
            int co = reg8[L] & 0x80 ? 0x10 : 0;
            reg8[L] = (reg8[L] << 1) + ci;
            reg8[L] &= 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCr_a: function() {
            int ci = reg8[A] & 0x80 ? 1 : 0;
            int co = reg8[A] & 0x80 ? 0x10 : 0;
            reg8[A] = (reg8[A] << 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RLCHL: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            int ci = i & 0x80 ? 1 : 0;
            int co = i & 0x80 ? 0x10 : 0;
            i = (i << 1) + ci;
            i &= 0xFF;
            reg8[F] = (i) ? 0 : 0x80;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 4;
        },

        RRr_b: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[B] & 1 ? 0x10 : 0;
            reg8[B] = (reg8[B] >> 1) + ci;
            reg8[B] &= 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRr_c: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[C] & 1 ? 0x10 : 0;
            reg8[C] = (reg8[C] >> 1) + ci;
            reg8[C] &= 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRr_d: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[D] & 1 ? 0x10 : 0;
            reg8[D] = (reg8[D] >> 1) + ci;
            reg8[D] &= 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRr_e: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[E] & 1 ? 0x10 : 0;
            reg8[E] = (reg8[E] >> 1) + ci;
            reg8[E] &= 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRr_h: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[H] & 1 ? 0x10 : 0;
            reg8[H] = (reg8[H] >> 1) + ci;
            reg8[H] &= 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRr_l: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[L] & 1 ? 0x10 : 0;
            reg8[L] = (reg8[L] >> 1) + ci;
            reg8[L] &= 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRr_a: function() {
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = reg8[A] & 1 ? 0x10 : 0;
            reg8[A] = (reg8[A] >> 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRHL: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            int ci = reg8[F] & 0x10 ? 0x80 : 0;
            int co = i & 1 ? 0x10 : 0;
            i = (i >> 1) + ci;
            i &= 0xFF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            reg8[F] = (i) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 4;
        },

        RRCr_b: function() {
            int ci = reg8[B] & 1 ? 0x80 : 0;
            int co = reg8[B] & 1 ? 0x10 : 0;
            reg8[B] = (reg8[B] >> 1) + ci;
            reg8[B] &= 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCr_c: function() {
            int ci = reg8[C] & 1 ? 0x80 : 0;
            int co = reg8[C] & 1 ? 0x10 : 0;
            reg8[C] = (reg8[C] >> 1) + ci;
            reg8[C] &= 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCr_d: function() {
            int ci = reg8[D] & 1 ? 0x80 : 0;
            int co = reg8[D] & 1 ? 0x10 : 0;
            reg8[D] = (reg8[D] >> 1) + ci;
            reg8[D] &= 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCr_e: function() {
            int ci = reg8[E] & 1 ? 0x80 : 0;
            int co = reg8[E] & 1 ? 0x10 : 0;
            reg8[E] = (reg8[E] >> 1) + ci;
            reg8[E] &= 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCr_h: function() {
            int ci = reg8[H] & 1 ? 0x80 : 0;
            int co = reg8[H] & 1 ? 0x10 : 0;
            reg8[H] = (reg8[H] >> 1) + ci;
            reg8[H] &= 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCr_l: function() {
            int ci = reg8[L] & 1 ? 0x80 : 0;
            int co = reg8[L] & 1 ? 0x10 : 0;
            reg8[L] = (reg8[L] >> 1) + ci;
            reg8[L] &= 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCr_a: function() {
            int ci = reg8[A] & 1 ? 0x80 : 0;
            int co = reg8[A] & 1 ? 0x10 : 0;
            reg8[A] = (reg8[A] >> 1) + ci;
            reg8[A] &= 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        RRCHL: function() {
            int i = mem8->read_byte((reg8[H] << 8) + reg8[L]);
            int ci = i & 1 ? 0x80 : 0;
            int co = i & 1 ? 0x10 : 0;
            i = (i >> 1) + ci;
            i &= 0xFF;
            mem8->write_byte((reg8[H] << 8) + reg8[L], i);
            reg8[F] = (i) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 4;
        },

        SLAr_b: function() {
            int co = reg8[B] & 0x80 ? 0x10 : 0;
            reg8[B] = (reg8[B] << 1) & 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLAr_c: function() {
            int co = reg8[C] & 0x80 ? 0x10 : 0;
            reg8[C] = (reg8[C] << 1) & 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLAr_d: function() {
            int co = reg8[D] & 0x80 ? 0x10 : 0;
            reg8[D] = (reg8[D] << 1) & 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLAr_e: function() {
            int co = reg8[E] & 0x80 ? 0x10 : 0;
            reg8[E] = (reg8[E] << 1) & 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLAr_h: function() {
            int co = reg8[H] & 0x80 ? 0x10 : 0;
            reg8[H] = (reg8[H] << 1) & 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLAr_l: function() {
            int co = reg8[L] & 0x80 ? 0x10 : 0;
            reg8[L] = (reg8[L] << 1) & 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLAr_a: function() {
            int co = reg8[A] & 0x80 ? 0x10 : 0;
            reg8[A] = (reg8[A] << 1) & 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },

        SLLr_b: function() {
            int co = reg8[B] & 0x80 ? 0x10 : 0;
            reg8[B] = (reg8[B] << 1) & 0xFF + 1;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLLr_c: function() {
            int co = reg8[C] & 0x80 ? 0x10 : 0;
            reg8[C] = (reg8[C] << 1) & 0xFF + 1;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLLr_d: function() {
            int co = reg8[D] & 0x80 ? 0x10 : 0;
            reg8[D] = (reg8[D] << 1) & 0xFF + 1;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLLr_e: function() {
            int co = reg8[E] & 0x80 ? 0x10 : 0;
            reg8[E] = (reg8[E] << 1) & 0xFF + 1;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLLr_h: function() {
            int co = reg8[H] & 0x80 ? 0x10 : 0;
            reg8[H] = (reg8[H] << 1) & 0xFF + 1;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLLr_l: function() {
            int co = reg8[L] & 0x80 ? 0x10 : 0;
            reg8[L] = (reg8[L] << 1) & 0xFF + 1;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SLLr_a: function() {
            int co = reg8[A] & 0x80 ? 0x10 : 0;
            reg8[A] = (reg8[A] << 1) & 0xFF + 1;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },

        SRAr_b: function() {
            int ci = reg8[B] & 0x80;
            int co = reg8[B] & 1 ? 0x10 : 0;
            reg8[B] = ((reg8[B] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRAr_c: function() {
            int ci = reg8[C] & 0x80;
            int co = reg8[C] & 1 ? 0x10 : 0;
            reg8[C] = ((reg8[C] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRAr_d: function() {
            int ci = reg8[D] & 0x80;
            int co = reg8[D] & 1 ? 0x10 : 0;
            reg8[D] = ((reg8[D] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRAr_e: function() {
            int ci = reg8[E] & 0x80;
            int co = reg8[E] & 1 ? 0x10 : 0;
            reg8[E] = ((reg8[E] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRAr_h: function() {
            int ci = reg8[H] & 0x80;
            int co = reg8[H] & 1 ? 0x10 : 0;
            reg8[H] = ((reg8[H] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRAr_l: function() {
            int ci = reg8[L] & 0x80;
            int co = reg8[L] & 1 ? 0x10 : 0;
            reg8[L] = ((reg8[L] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRAr_a: function() {
            int ci = reg8[A] & 0x80;
            int co = reg8[A] & 1 ? 0x10 : 0;
            reg8[A] = ((reg8[A] >> 1) + ci) & 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },

        SRLr_b: function() {
            int co = reg8[B] & 1 ? 0x10 : 0;
            reg8[B] = (reg8[B] >> 1) & 0xFF;
            reg8[F] = (reg8[B]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRLr_c: function() {
            int co = reg8[C] & 1 ? 0x10 : 0;
            reg8[C] = (reg8[C] >> 1) & 0xFF;
            reg8[F] = (reg8[C]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRLr_d: function() {
            int co = reg8[D] & 1 ? 0x10 : 0;
            reg8[D] = (reg8[D] >> 1) & 0xFF;
            reg8[F] = (reg8[D]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRLr_e: function() {
            int co = reg8[E] & 1 ? 0x10 : 0;
            reg8[E] = (reg8[E] >> 1) & 0xFF;
            reg8[F] = (reg8[E]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRLr_h: function() {
            int co = reg8[H] & 1 ? 0x10 : 0;
            reg8[H] = (reg8[H] >> 1) & 0xFF;
            reg8[F] = (reg8[H]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRLr_l: function() {
            int co = reg8[L] & 1 ? 0x10 : 0;
            reg8[L] = (reg8[L] >> 1) & 0xFF;
            reg8[F] = (reg8[L]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },
        SRLr_a: function() {
            int co = reg8[A] & 1 ? 0x10 : 0;
            reg8[A] = (reg8[A] >> 1) & 0xFF;
            reg8[F] = (reg8[A]) ? 0 : 0x80;
            reg8[F] = (reg8[F] & 0xEF) + co;
            m_cycle = 2;
        },

        CPL: function() {
            reg8[A] ^= 0xFF;
            reg8[F] = reg8[A] ? 0 : 0x80;
            m_cycle = 1;
        },
        NEG: function() {
            reg8[A] = 0 - reg8[A];
            reg8[F] = (reg8[A] < 0) ? 0x10 : 0;
            reg8[A] &= 0xFF;
            if (!reg8[A]) reg8[F] |= 0x80;
            m_cycle = 2;
        },

        CCF: function() {
            int ci = reg8[F] & 0x10 ? 0 : 0x10;
            reg8[F] = (reg8[F] & 0xEF) + ci;
            m_cycle = 1;
        },
        SCF: function() {
            reg8[F] |= 0x10;
            m_cycle = 1;
        },

        /*--- Stack ---*/
        PUSHBC: function() {
            sp--;
            mem8->write_byte(sp, reg8[B]);
            sp--;
            mem8->write_byte(sp, reg8[C]);
            m_cycle = 3;
        },
        PUSHDE: function() {
            sp--;
            mem8->write_byte(sp, reg8[D]);
            sp--;
            mem8->write_byte(sp, reg8[E]);
            m_cycle = 3;
        },
        PUSHHL: function() {
            sp--;
            mem8->write_byte(sp, reg8[H]);
            sp--;
            mem8->write_byte(sp, reg8[L]);
            m_cycle = 3;
        },
        PUSHAF: function() {
            sp--;
            mem8->write_byte(sp, reg8[A]);
            sp--;
            mem8->write_byte(sp, reg8[F]);
            m_cycle = 3;
        },

        POPBC: function() {
            reg8[C] = mem8->read_byte(sp);
            sp++;
            reg8[B] = mem8->read_byte(sp);
            sp++;
            m_cycle = 3;
        },
        POPDE: function() {
            reg8[E] = mem8->read_byte(sp);
            sp++;
            reg8[D] = mem8->read_byte(sp);
            sp++;
            m_cycle = 3;
        },
        POPHL: function() {
            reg8[L] = mem8->read_byte(sp);
            sp++;
            reg8[H] = mem8->read_byte(sp);
            sp++;
            m_cycle = 3;
        },
        POPAF: function() {
            reg8[F] = mem8->read_byte(sp);
            sp++;
            reg8[A] = mem8->read_byte(sp);
            sp++;
            m_cycle = 3;
        },

        /*--- Jump ---*/
        JPnn: function() {
            pc = mem8->read_word(pc);
            m_cycle = 3;
        },
        JPHL: function() {
            pc = (reg8[H] << 8) + reg8[L];
            m_cycle = 1;
        },
        JPNZnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x80) == 0x00) {
                pc = mem8->read_word(pc);
                m_cycle++;
            } else pc += 2;
        },
        JPZnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x80) == 0x80) {
                pc = mem8->read_word(pc);
                m_cycle++;
            } else pc += 2;
        },
        JPNCnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x10) == 0x00) {
                pc = mem8->read_word(pc);
                m_cycle++;
            } else pc += 2;
        },
        JPCnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x10) == 0x10) {
                pc = mem8->read_word(pc);
                m_cycle++;
            } else pc += 2;
        },

        JRn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            m_cycle = 2;
            pc += i;
            m_cycle++;
        },
        JRNZn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            m_cycle = 2;
            if ((reg8[F] & 0x80) == 0x00) {
                pc += i;
                m_cycle++;
            }
        },
        JRZn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            m_cycle = 2;
            if ((reg8[F] & 0x80) == 0x80) {
                pc += i;
                m_cycle++;
            }
        },
        JRNCn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            m_cycle = 2;
            if ((reg8[F] & 0x10) == 0x00) {
                pc += i;
                m_cycle++;
            }
        },
        JRCn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            m_cycle = 2;
            if ((reg8[F] & 0x10) == 0x10) {
                pc += i;
                m_cycle++;
            }
        },

        DJNZn: function() {
            int i = mem8->read_byte(pc);
            if (i > 127) i = -((~i + 1) & 0xFF);
            pc++;
            m_cycle = 2;
            reg8[B]--;
            if (reg8[B]) {
                pc += i;
                m_cycle++;
            }
        },

        CALLnn: function() {
            sp -= 2;
            mem8->write_word(sp, pc + 2);
            pc = mem8->read_word(pc);
            m_cycle = 5;
        },
        CALLNZnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x80) == 0x00) {
                sp -= 2;
                mem8->write_word(sp, pc + 2);
                pc = mem8->read_word(pc);
                m_cycle += 2;
            } else pc += 2;
        },
        CALLZnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x80) == 0x80) {
                sp -= 2;
                mem8->write_word(sp, pc + 2);
                pc = mem8->read_word(pc);
                m_cycle += 2;
            } else pc += 2;
        },
        CALLNCnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x10) == 0x00) {
                sp -= 2;
                mem8->write_word(sp, pc + 2);
                pc = mem8->read_word(pc);
                m_cycle += 2;
            } else pc += 2;
        },
        CALLCnn: function() {
            m_cycle = 3;
            if ((reg8[F] & 0x10) == 0x10) {
                sp -= 2;
                mem8->write_word(sp, pc + 2);
                pc = mem8->read_word(pc);
                m_cycle += 2;
            } else pc += 2;
        },

        RET: function() {
            pc = mem8->read_word(sp);
            sp += 2;
            m_cycle = 3;
        },
        RETI: function() {
            Z80._r.ime = 1;
            Z80._ops.rrs();
            pc = mem8->read_word(sp);
            sp += 2;
            m_cycle = 3;
        },
        RETNZ: function() {
            m_cycle = 1;
            if ((reg8[F] & 0x80) == 0x00) {
                pc = mem8->read_word(sp);
                sp += 2;
                m_cycle += 2;
            }
        },
        RETZ: function() {
            m_cycle = 1;
            if ((reg8[F] & 0x80) == 0x80) {
                pc = mem8->read_word(sp);
                sp += 2;
                m_cycle += 2;
            }
        },
        RETNC: function() {
            m_cycle = 1;
            if ((reg8[F] & 0x10) == 0x00) {
                pc = mem8->read_word(sp);
                sp += 2;
                m_cycle += 2;
            }
        },
        RETC: function() {
            m_cycle = 1;
            if ((reg8[F] & 0x10) == 0x10) {
                pc = mem8->read_word(sp);
                sp += 2;
                m_cycle += 2;
            }
        },

        RST00: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x00;
            m_cycle = 3;
        },
        RST08: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x08;
            m_cycle = 3;
        },
        RST10: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x10;
            m_cycle = 3;
        },
        RST18: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x18;
            m_cycle = 3;
        },
        RST20: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x20;
            m_cycle = 3;
        },
        RST28: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x28;
            m_cycle = 3;
        },
        RST30: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x30;
            m_cycle = 3;
        },
        RST38: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x38;
            m_cycle = 3;
        },
        RST40: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x40;
            m_cycle = 3;
        },
        RST48: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x48;
            m_cycle = 3;
        },
        RST50: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x50;
            m_cycle = 3;
        },
        RST58: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x58;
            m_cycle = 3;
        },
        RST60: function() {
            Z80._ops.rsv();
            sp -= 2;
            mem8->write_word(sp, pc);
            pc = 0x60;
            m_cycle = 3;
        },

        NOP: function() {
            m_cycle = 1;
        },
        HALT: function() {
            Z80._halt = 1;
            m_cycle = 1;
        },

        DI: function() {
            Z80._r.ime = 0;
            m_cycle = 1;
        },
        EI: function() {
            Z80._r.ime = 1;
            m_cycle = 1;
        },

        /*--- Helper functions ---*/
        rsv: function() {
            Z80._rsv.a = reg8[A];
            Z80._rsv.b = reg8[B];
            Z80._rsv.c = reg8[C];
            Z80._rsv.d = reg8[D];
            Z80._rsv.e = reg8[E];
            Z80._rsv.f = reg8[F];
            Z80._rsv.h = reg8[H];
            Z80._rsv.l = reg8[L];
        },

        rrs: function() {
            reg8[A] = Z80._rsv.a;
            reg8[B] = Z80._rsv.b;
            reg8[C] = Z80._rsv.c;
            reg8[D] = Z80._rsv.d;
            reg8[E] = Z80._rsv.e;
            reg8[F] = Z80._rsv.f;
            reg8[H] = Z80._rsv.h;
            reg8[L] = Z80._rsv.l;
        },

        MAPcb: function() {
            int i = mem8->read_byte(pc);
            pc++;
            pc &= 0xFFFF;
            if (Z80._cbmap[i]) Z80._cbmap[i]();
            else console.log(i);
        },

        XX: function() {
            /*Undefined map entry*/
            int opc = pc - 1;
            LOG.out('Z80', 'Unimplemented instruction at $' + opc.toString(16) + ', stopping.');
            Z80._stop = 1;
        }
    },

    _map: [],
    _cbmap: []
};

Z80._map = [
    // 00
    Z80._ops.NOP, Z80._ops.LDBCnn, Z80._ops.LDBCmA, Z80._ops.INCBC,
    Z80._ops.INCr_b, Z80._ops.DECr_b, Z80._ops.LDrn_b, Z80._ops.RLCA,
    Z80._ops.LDmmSP, Z80._ops.ADDHLBC, Z80._ops.LDABCm, Z80._ops.DECBC,
    Z80._ops.INCr_c, Z80._ops.DECr_c, Z80._ops.LDrn_c, Z80._ops.RRCA,
    // 10
    Z80._ops.DJNZn, Z80._ops.LDDEnn, Z80._ops.LDDEmA, Z80._ops.INCDE,
    Z80._ops.INCr_d, Z80._ops.DECr_d, Z80._ops.LDrn_d, Z80._ops.RLA,
    Z80._ops.JRn, Z80._ops.ADDHLDE, Z80._ops.LDADEm, Z80._ops.DECDE,
    Z80._ops.INCr_e, Z80._ops.DECr_e, Z80._ops.LDrn_e, Z80._ops.RRA,
    // 20
    Z80._ops.JRNZn, Z80._ops.LDHLnn, Z80._ops.LDHLIA, Z80._ops.INCHL,
    Z80._ops.INCr_h, Z80._ops.DECr_h, Z80._ops.LDrn_h, Z80._ops.DAA,
    Z80._ops.JRZn, Z80._ops.ADDHLHL, Z80._ops.LDAHLI, Z80._ops.DECHL,
    Z80._ops.INCr_l, Z80._ops.DECr_l, Z80._ops.LDrn_l, Z80._ops.CPL,
    // 30
    Z80._ops.JRNCn, Z80._ops.LDSPnn, Z80._ops.LDHLDA, Z80._ops.INCSP,
    Z80._ops.INCHLm, Z80._ops.DECHLm, Z80._ops.LDHLmn, Z80._ops.SCF,
    Z80._ops.JRCn, Z80._ops.ADDHLSP, Z80._ops.LDAHLD, Z80._ops.DECSP,
    Z80._ops.INCr_a, Z80._ops.DECr_a, Z80._ops.LDrn_a, Z80._ops.CCF,
    // 40
    Z80._ops.LDrr_bb, Z80._ops.LDrr_bc, Z80._ops.LDrr_bd, Z80._ops.LDrr_be,
    Z80._ops.LDrr_bh, Z80._ops.LDrr_bl, Z80._ops.LDrHLm_b, Z80._ops.LDrr_ba,
    Z80._ops.LDrr_cb, Z80._ops.LDrr_cc, Z80._ops.LDrr_cd, Z80._ops.LDrr_ce,
    Z80._ops.LDrr_ch, Z80._ops.LDrr_cl, Z80._ops.LDrHLm_c, Z80._ops.LDrr_ca,
    // 50
    Z80._ops.LDrr_db, Z80._ops.LDrr_dc, Z80._ops.LDrr_dd, Z80._ops.LDrr_de,
    Z80._ops.LDrr_dh, Z80._ops.LDrr_dl, Z80._ops.LDrHLm_d, Z80._ops.LDrr_da,
    Z80._ops.LDrr_eb, Z80._ops.LDrr_ec, Z80._ops.LDrr_ed, Z80._ops.LDrr_ee,
    Z80._ops.LDrr_eh, Z80._ops.LDrr_el, Z80._ops.LDrHLm_e, Z80._ops.LDrr_ea,
    // 60
    Z80._ops.LDrr_hb, Z80._ops.LDrr_hc, Z80._ops.LDrr_hd, Z80._ops.LDrr_he,
    Z80._ops.LDrr_hh, Z80._ops.LDrr_hl, Z80._ops.LDrHLm_h, Z80._ops.LDrr_ha,
    Z80._ops.LDrr_lb, Z80._ops.LDrr_lc, Z80._ops.LDrr_ld, Z80._ops.LDrr_le,
    Z80._ops.LDrr_lh, Z80._ops.LDrr_ll, Z80._ops.LDrHLm_l, Z80._ops.LDrr_la,
    // 70
    Z80._ops.LDHLmr_b, Z80._ops.LDHLmr_c, Z80._ops.LDHLmr_d, Z80._ops.LDHLmr_e,
    Z80._ops.LDHLmr_h, Z80._ops.LDHLmr_l, Z80._ops.HALT, Z80._ops.LDHLmr_a,
    Z80._ops.LDrr_ab, Z80._ops.LDrr_ac, Z80._ops.LDrr_ad, Z80._ops.LDrr_ae,
    Z80._ops.LDrr_ah, Z80._ops.LDrr_al, Z80._ops.LDrHLm_a, Z80._ops.LDrr_aa,
    // 80
    Z80._ops.ADDr_b, Z80._ops.ADDr_c, Z80._ops.ADDr_d, Z80._ops.ADDr_e,
    Z80._ops.ADDr_h, Z80._ops.ADDr_l, Z80._ops.ADDHL, Z80._ops.ADDr_a,
    Z80._ops.ADCr_b, Z80._ops.ADCr_c, Z80._ops.ADCr_d, Z80._ops.ADCr_e,
    Z80._ops.ADCr_h, Z80._ops.ADCr_l, Z80._ops.ADCHL, Z80._ops.ADCr_a,
    // 90
    Z80._ops.SUBr_b, Z80._ops.SUBr_c, Z80._ops.SUBr_d, Z80._ops.SUBr_e,
    Z80._ops.SUBr_h, Z80._ops.SUBr_l, Z80._ops.SUBHL, Z80._ops.SUBr_a,
    Z80._ops.SBCr_b, Z80._ops.SBCr_c, Z80._ops.SBCr_d, Z80._ops.SBCr_e,
    Z80._ops.SBCr_h, Z80._ops.SBCr_l, Z80._ops.SBCHL, Z80._ops.SBCr_a,
    // A0
    Z80._ops.ANDr_b, Z80._ops.ANDr_c, Z80._ops.ANDr_d, Z80._ops.ANDr_e,
    Z80._ops.ANDr_h, Z80._ops.ANDr_l, Z80._ops.ANDHL, Z80._ops.ANDr_a,
    Z80._ops.XORr_b, Z80._ops.XORr_c, Z80._ops.XORr_d, Z80._ops.XORr_e,
    Z80._ops.XORr_h, Z80._ops.XORr_l, Z80._ops.XORHL, Z80._ops.XORr_a,
    // B0
    Z80._ops.ORr_b, Z80._ops.ORr_c, Z80._ops.ORr_d, Z80._ops.ORr_e,
    Z80._ops.ORr_h, Z80._ops.ORr_l, Z80._ops.ORHL, Z80._ops.ORr_a,
    Z80._ops.CPr_b, Z80._ops.CPr_c, Z80._ops.CPr_d, Z80._ops.CPr_e,
    Z80._ops.CPr_h, Z80._ops.CPr_l, Z80._ops.CPHL, Z80._ops.CPr_a,
    // C0
    Z80._ops.RETNZ, Z80._ops.POPBC, Z80._ops.JPNZnn, Z80._ops.JPnn,
    Z80._ops.CALLNZnn, Z80._ops.PUSHBC, Z80._ops.ADDn, Z80._ops.RST00,
    Z80._ops.RETZ, Z80._ops.RET, Z80._ops.JPZnn, Z80._ops.MAPcb,
    Z80._ops.CALLZnn, Z80._ops.CALLnn, Z80._ops.ADCn, Z80._ops.RST08,
    // D0
    Z80._ops.RETNC, Z80._ops.POPDE, Z80._ops.JPNCnn, Z80._ops.XX,
    Z80._ops.CALLNCnn, Z80._ops.PUSHDE, Z80._ops.SUBn, Z80._ops.RST10,
    Z80._ops.RETC, Z80._ops.RETI, Z80._ops.JPCnn, Z80._ops.XX,
    Z80._ops.CALLCnn, Z80._ops.XX, Z80._ops.SBCn, Z80._ops.RST18,
    // E0
    Z80._ops.LDIOnA, Z80._ops.POPHL, Z80._ops.LDIOCA, Z80._ops.XX,
    Z80._ops.XX, Z80._ops.PUSHHL, Z80._ops.ANDn, Z80._ops.RST20,
    Z80._ops.ADDSPn, Z80._ops.JPHL, Z80._ops.LDmmA, Z80._ops.XX,
    Z80._ops.XX, Z80._ops.XX, Z80._ops.XORn, Z80._ops.RST28,
    // F0
    Z80._ops.LDAIOn, Z80._ops.POPAF, Z80._ops.LDAIOC, Z80._ops.DI,
    Z80._ops.XX, Z80._ops.PUSHAF, Z80._ops.ORn, Z80._ops.RST30,
    Z80._ops.LDHLSPn, Z80._ops.XX, Z80._ops.LDAmm, Z80._ops.EI,
    Z80._ops.XX, Z80._ops.XX, Z80._ops.CPn, Z80._ops.RST38
];

Z80._cbmap = [
    // CB00
    Z80._ops.RLCr_b, Z80._ops.RLCr_c, Z80._ops.RLCr_d, Z80._ops.RLCr_e,
    Z80._ops.RLCr_h, Z80._ops.RLCr_l, Z80._ops.RLCHL, Z80._ops.RLCr_a,
    Z80._ops.RRCr_b, Z80._ops.RRCr_c, Z80._ops.RRCr_d, Z80._ops.RRCr_e,
    Z80._ops.RRCr_h, Z80._ops.RRCr_l, Z80._ops.RRCHL, Z80._ops.RRCr_a,
    // CB10
    Z80._ops.RLr_b, Z80._ops.RLr_c, Z80._ops.RLr_d, Z80._ops.RLr_e,
    Z80._ops.RLr_h, Z80._ops.RLr_l, Z80._ops.RLHL, Z80._ops.RLr_a,
    Z80._ops.RRr_b, Z80._ops.RRr_c, Z80._ops.RRr_d, Z80._ops.RRr_e,
    Z80._ops.RRr_h, Z80._ops.RRr_l, Z80._ops.RRHL, Z80._ops.RRr_a,
    // CB20
    Z80._ops.SLAr_b, Z80._ops.SLAr_c, Z80._ops.SLAr_d, Z80._ops.SLAr_e,
    Z80._ops.SLAr_h, Z80._ops.SLAr_l, Z80._ops.XX, Z80._ops.SLAr_a,
    Z80._ops.SRAr_b, Z80._ops.SRAr_c, Z80._ops.SRAr_d, Z80._ops.SRAr_e,
    Z80._ops.SRAr_h, Z80._ops.SRAr_l, Z80._ops.XX, Z80._ops.SRAr_a,
    // CB30
    Z80._ops.SWAPr_b, Z80._ops.SWAPr_c, Z80._ops.SWAPr_d, Z80._ops.SWAPr_e,
    Z80._ops.SWAPr_h, Z80._ops.SWAPr_l, Z80._ops.XX, Z80._ops.SWAPr_a,
    Z80._ops.SRLr_b, Z80._ops.SRLr_c, Z80._ops.SRLr_d, Z80._ops.SRLr_e,
    Z80._ops.SRLr_h, Z80._ops.SRLr_l, Z80._ops.XX, Z80._ops.SRLr_a,
    // CB40
    Z80._ops.BIT0b, Z80._ops.BIT0c, Z80._ops.BIT0d, Z80._ops.BIT0e,
    Z80._ops.BIT0h, Z80._ops.BIT0l, Z80._ops.BIT0m, Z80._ops.BIT0a,
    Z80._ops.BIT1b, Z80._ops.BIT1c, Z80._ops.BIT1d, Z80._ops.BIT1e,
    Z80._ops.BIT1h, Z80._ops.BIT1l, Z80._ops.BIT1m, Z80._ops.BIT1a,
    // CB50
    Z80._ops.BIT2b, Z80._ops.BIT2c, Z80._ops.BIT2d, Z80._ops.BIT2e,
    Z80._ops.BIT2h, Z80._ops.BIT2l, Z80._ops.BIT2m, Z80._ops.BIT2a,
    Z80._ops.BIT3b, Z80._ops.BIT3c, Z80._ops.BIT3d, Z80._ops.BIT3e,
    Z80._ops.BIT3h, Z80._ops.BIT3l, Z80._ops.BIT3m, Z80._ops.BIT3a,
    // CB60
    Z80._ops.BIT4b, Z80._ops.BIT4c, Z80._ops.BIT4d, Z80._ops.BIT4e,
    Z80._ops.BIT4h, Z80._ops.BIT4l, Z80._ops.BIT4m, Z80._ops.BIT4a,
    Z80._ops.BIT5b, Z80._ops.BIT5c, Z80._ops.BIT5d, Z80._ops.BIT5e,
    Z80._ops.BIT5h, Z80._ops.BIT5l, Z80._ops.BIT5m, Z80._ops.BIT5a,
    // CB70
    Z80._ops.BIT6b, Z80._ops.BIT6c, Z80._ops.BIT6d, Z80._ops.BIT6e,
    Z80._ops.BIT6h, Z80._ops.BIT6l, Z80._ops.BIT6m, Z80._ops.BIT6a,
    Z80._ops.BIT7b, Z80._ops.BIT7c, Z80._ops.BIT7d, Z80._ops.BIT7e,
    Z80._ops.BIT7h, Z80._ops.BIT7l, Z80._ops.BIT7m, Z80._ops.BIT7a,
    // CB80
    Z80._ops.RES0b, Z80._ops.RES0c, Z80._ops.RES0d, Z80._ops.RES0e,
    Z80._ops.RES0h, Z80._ops.RES0l, Z80._ops.RES0m, Z80._ops.RES0a,
    Z80._ops.RES1b, Z80._ops.RES1c, Z80._ops.RES1d, Z80._ops.RES1e,
    Z80._ops.RES1h, Z80._ops.RES1l, Z80._ops.RES1m, Z80._ops.RES1a,
    // CB90
    Z80._ops.RES2b, Z80._ops.RES2c, Z80._ops.RES2d, Z80._ops.RES2e,
    Z80._ops.RES2h, Z80._ops.RES2l, Z80._ops.RES2m, Z80._ops.RES2a,
    Z80._ops.RES3b, Z80._ops.RES3c, Z80._ops.RES3d, Z80._ops.RES3e,
    Z80._ops.RES3h, Z80._ops.RES3l, Z80._ops.RES3m, Z80._ops.RES3a,
    // CBA0
    Z80._ops.RES4b, Z80._ops.RES4c, Z80._ops.RES4d, Z80._ops.RES4e,
    Z80._ops.RES4h, Z80._ops.RES4l, Z80._ops.RES4m, Z80._ops.RES4a,
    Z80._ops.RES5b, Z80._ops.RES5c, Z80._ops.RES5d, Z80._ops.RES5e,
    Z80._ops.RES5h, Z80._ops.RES5l, Z80._ops.RES5m, Z80._ops.RES5a,
    // CBB0
    Z80._ops.RES6b, Z80._ops.RES6c, Z80._ops.RES6d, Z80._ops.RES6e,
    Z80._ops.RES6h, Z80._ops.RES6l, Z80._ops.RES6m, Z80._ops.RES6a,
    Z80._ops.RES7b, Z80._ops.RES7c, Z80._ops.RES7d, Z80._ops.RES7e,
    Z80._ops.RES7h, Z80._ops.RES7l, Z80._ops.RES7m, Z80._ops.RES7a,
    // CBC0
    Z80._ops.SET0b, Z80._ops.SET0c, Z80._ops.SET0d, Z80._ops.SET0e,
    Z80._ops.SET0h, Z80._ops.SET0l, Z80._ops.SET0m, Z80._ops.SET0a,
    Z80._ops.SET1b, Z80._ops.SET1c, Z80._ops.SET1d, Z80._ops.SET1e,
    Z80._ops.SET1h, Z80._ops.SET1l, Z80._ops.SET1m, Z80._ops.SET1a,
    // CBD0
    Z80._ops.SET2b, Z80._ops.SET2c, Z80._ops.SET2d, Z80._ops.SET2e,
    Z80._ops.SET2h, Z80._ops.SET2l, Z80._ops.SET2m, Z80._ops.SET2a,
    Z80._ops.SET3b, Z80._ops.SET3c, Z80._ops.SET3d, Z80._ops.SET3e,
    Z80._ops.SET3h, Z80._ops.SET3l, Z80._ops.SET3m, Z80._ops.SET3a,
    // CBE0
    Z80._ops.SET4b, Z80._ops.SET4c, Z80._ops.SET4d, Z80._ops.SET4e,
    Z80._ops.SET4h, Z80._ops.SET4l, Z80._ops.SET4m, Z80._ops.SET4a,
    Z80._ops.SET5b, Z80._ops.SET5c, Z80._ops.SET5d, Z80._ops.SET5e,
    Z80._ops.SET5h, Z80._ops.SET5l, Z80._ops.SET5m, Z80._ops.SET5a,
    // CBF0
    Z80._ops.SET6b, Z80._ops.SET6c, Z80._ops.SET6d, Z80._ops.SET6e,
    Z80._ops.SET6h, Z80._ops.SET6l, Z80._ops.SET6m, Z80._ops.SET6a,
    Z80._ops.SET7b, Z80._ops.SET7c, Z80._ops.SET7d, Z80._ops.SET7e,
    Z80._ops.SET7h, Z80._ops.SET7l, Z80._ops.SET7m, Z80._ops.SET7a,
];
