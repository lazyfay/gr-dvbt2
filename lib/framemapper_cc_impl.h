/* -*- c++ -*- */
/* 
 * Copyright 2014 Ron Economos.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_DVBT2_FRAMEMAPPER_CC_IMPL_H
#define INCLUDED_DVBT2_FRAMEMAPPER_CC_IMPL_H

#include <dvbt2/framemapper_cc.h>

#define KBCH_1_4 3072
#define NBCH_1_4 3240
#define KBCH_1_2 7032
#define NBCH_1_2 7200
#define BITS_PER_GROUP 360
#define BITS_LAST_GROUP 192

typedef struct{
    int type;
    int bwt_ext;
    int s1;
    int s2;
    int l1_repetition_flag;
    int guard_interval;
    int papr;
    int l1_mod;
    int l1_cod;
    int l1_fec_type;
    int l1_post_size;
    int l1_post_info_size;
    int pilot_pattern;
    int tx_id_availability;
    int cell_id;
    int network_id;
    int t2_system_id;
    int num_t2_frames;
    int num_data_symbols;
    int regen_flag;
    int l1_post_extension;
    int num_rf;
    int current_rf_index;
    int t2_version;
    int l1_post_scrambled;
    int t2_base_lite;
    int reserved;
    int crc_32;
}L1Pre;

typedef struct{
    int sub_slices_per_frame;
    int num_plp;
    int num_aux;
    int aux_config_rfu;
    int rf_idx;
    int frequency;
    int plp_id;
    int plp_type;
    int plp_payload_type;
    int ff_flag;
    int first_rf_idx;
    int first_frame_idx;
    int plp_group_id;
    int plp_cod;
    int plp_mod;
    int plp_rotation;
    int plp_fec_type;
    int plp_num_blocks_max;
    int frame_interval;
    int time_il_length;
    int time_il_type;
    int in_band_a_flag;
    int in_band_b_flag;
    int reserved_1;
    int plp_mode;
    int static_flag;
    int static_padding_flag;
    int fef_length_msb;
    int reserved_2;
    int frame_idx;
    int sub_slice_interval;
    int type_2_start;
    int l1_change_counter;
    int start_rf_idx;
    int reserved_3;
    int plp_id_dynamic;
    int plp_start;
    int plp_num_blocks;
    int reserved_4;
    int reserved_5;
}L1Post;

typedef struct{
   L1Pre l1pre_data;
   L1Post l1post_data;
}L1Signalling;

typedef struct{
    int table_length;
    int d[LDPC_ENCODE_TABLE_LENGTH];
    int p[LDPC_ENCODE_TABLE_LENGTH];
}l1_ldpc_encode_table;

namespace gr {
  namespace dvbt2 {

    class framemapper_cc_impl : public framemapper_cc
    {
     private:
      int cell_size;
      int mapped_items;
      int l1_constellation;
      L1Signalling L1_Signalling[1];
      void add_l1pre(gr_complex *);
      void add_l1post(unsigned char *);
      int add_crc32_bits(unsigned char *, int);
      unsigned int m_poly_s_12[6];
      int poly_mult(const int*, int, const int*, int, int*);
      void poly_pack(const int*, unsigned int*, int);
      void poly_reverse(int*, int*, int);
      inline void reg_6_shift(unsigned int*);
      void bch_poly_build_tables(void);
      void l1pre_ldpc_lookup_generate(void);
      void l1post_ldpc_lookup_generate(void);
      l1_ldpc_encode_table ldpc_encode;
      unsigned char l1_temp[FRAME_SIZE_SHORT];
      gr_complex l1pre_cache[1840];
      gr_complex m_bpsk[2];
      gr_complex m_qpsk[4];
      gr_complex m_16qam[16];
      gr_complex m_64qam[64];

      const static int ldpc_tab_1_4S[9][13];
      const static int ldpc_tab_1_2S[20][9];

     public:
      framemapper_cc_impl(dvbt2_constellation_t constellation, dvbt2_framesize_t framesize, int fecblocks, int tiblocks, dvbt2_extended_carrier_t carriermode, dvbt2_fftsize_t fftsize, dvbt2_guardinterval_t guardinterval, dvbt2_l1constellation_t l1constellation, dvbt2_pilotpattern_t pilotpattern);
      ~framemapper_cc_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);
    };

  } // namespace dvbt2
} // namespace gr

#endif /* INCLUDED_DVBT2_FRAMEMAPPER_CC_IMPL_H */
