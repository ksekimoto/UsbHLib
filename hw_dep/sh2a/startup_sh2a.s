############################################################################
## Copyright (c) 2010-2014 Kentaro Sekimoto  All rights reserved.
############################################################################

#	.text

	.extern		_main
	.extern		_CPU_IRQInterrupt
	.extern		_stack
    .global     _startup

    .section    .isr_vector
    .align  4
    .org    0x00000000
_vectors:
    .long   _startup    # 000 0x000 Power On Reset
    .long   _stack      # 001 0x004 Stack
    .long   _startup    # 002 0x008 Manual Reset
    .long   _stack      # 003 0x00c Stack
    .long   _intr_004   # 004 0x010 Illegal instruction
    .long   _intr_005   # 005 0x014 System reserved
    .long   _intr_006   # 006 0x018 Slot instruction
    .long   _intr_007   # 007 0x01c System reserved
    .long   _vectors    # 008 0x020 Intr_008 is reserved for SPI-FlashBoot Address
    .long   _intr_009   # 009 0x024 CPU address error
    .long   _intr_010   # 010 0x028 DMA address error
    .long   _intr_011   # 011 0x02c NMI
    .long   _intr_012   # 012 0x030 System reserved
    .long   _intr_013   # 013 0x034 FPU exception
    .long   _intr_014   # 014 0x038 UDI
    .long   _intr_015   # 015 0x03c Bank overflow
    .long   _intr_016   # 016 0x040 Bank underflow
    .long   _intr_017   # 017 0x044 Division by zero
    .long   _intr_018   # 018 0x048 Overflow
    .long   _intr_019   # 019
    .long   _intr_020   # 020
    .long   _intr_021   # 021
    .long   _intr_022   # 022
    .long   _intr_023   # 023
    .long   _intr_024   # 024
    .long   _intr_025   # 025
    .long   _intr_026   # 026
    .long   _intr_027   # 027
    .long   _intr_028   # 028
    .long   _intr_029   # 029
    .long   _intr_030   # 030
    .long   _intr_031   # 031
    .long   _intr_032   # 032 0x080 Trap #32 is reserved for USE_GDBSTUB.
    .long   _intr_033   # 033
    .long   _intr_034   # 034
    .long   _intr_035   # 035
    .long   _intr_036   # 036
    .long   _intr_037   # 037
    .long   _intr_038   # 038
    .long   _intr_039   # 039
    .long   _intr_040   # 040
    .long   _intr_041   # 041
    .long   _intr_042   # 042
    .long   _intr_043   # 043
    .long   _intr_044   # 044
    .long   _intr_045   # 045
    .long   _intr_046   # 046
    .long   _intr_047   # 047
    .long   _intr_048   # 048
    .long   _intr_049   # 049
    .long   _intr_050   # 050
    .long   _intr_051   # 051
    .long   _intr_052   # 052
    .long   _intr_053   # 053
    .long   _intr_054   # 054
    .long   _intr_055   # 055
    .long   _intr_056   # 056
    .long   _intr_057   # 057
    .long   _intr_058   # 058
    .long   _intr_059   # 059
    .long   _intr_060   # 060
    .long   _intr_061   # 061
    .long   _intr_062   # 062
    .long   _intr_063   # 063
    .long   _intr_064   # 064
    .long   _intr_065   # 065
    .long   _intr_066   # 066
    .long   _intr_067   # 067
    .long   _intr_068   # 068
    .long   _intr_069   # 069
    .long   _intr_070   # 070
    .long   _intr_071   # 071
    .long   _intr_072   # 072
    .long   _intr_073   # 073
    .long   _intr_074   # 074
    .long   _intr_075   # 075
    .long   _intr_076   # 076
    .long   _intr_077   # 077
    .long   _intr_078   # 078
    .long   _intr_079   # 079
    .long   _intr_080   # 080
    .long   _intr_081   # 081
    .long   _intr_082   # 082
    .long   _intr_083   # 083
    .long   _intr_084   # 084
    .long   _intr_085   # 085
    .long   _intr_086   # 086
    .long   _intr_087   # 087
    .long   _intr_088   # 088
    .long   _intr_089   # 089
    .long   _intr_090   # 090
    .long   _intr_091   # 091
    .long   _intr_092   # 092
    .long   _intr_093   # 093
    .long   _intr_094   # 094
    .long   _intr_095   # 095
    .long   _intr_096   # 096
    .long   _intr_097   # 097
    .long   _intr_098   # 098
    .long   _intr_099   # 099
    .long   _intr_100   # 100
    .long   _intr_101   # 101
    .long   _intr_102   # 102
    .long   _intr_103   # 103
    .long   _intr_104   # 104
    .long   _intr_105   # 105
    .long   _intr_106   # 106
    .long   _intr_107   # 107
    .long   _intr_108   # 108
    .long   _intr_109   # 109
    .long   _intr_110   # 110
    .long   _intr_111   # 111
    .long   _intr_112   # 112
    .long   _intr_113   # 113
    .long   _intr_114   # 114
    .long   _intr_115   # 115
    .long   _intr_116   # 116
    .long   _intr_117   # 117
    .long   _intr_118   # 118
    .long   _intr_119   # 119
    .long   _intr_120   # 120
    .long   _intr_121   # 121
    .long   _intr_122   # 122
    .long   _intr_123   # 123
    .long   _intr_124   # 124
    .long   _intr_125   # 125
    .long   _intr_126   # 126
    .long   _intr_127   # 127
    .long   _intr_128   # 128
    .long   _intr_129   # 129
    .long   _intr_130   # 130
    .long   _intr_131   # 131
    .long   _intr_132   # 132
    .long   _intr_133   # 133
    .long   _intr_134   # 134
    .long   _intr_135   # 135
    .long   _intr_136   # 136
    .long   _intr_137   # 137
    .long   _intr_138   # 138
    .long   _intr_139   # 139
    .long   _intr_140   # 140
    .long   _intr_141   # 141
    .long   _intr_142   # 142
    .long   _intr_143   # 143
    .long   _intr_144   # 144
    .long   _intr_145   # 145
    .long   _intr_146   # 146
    .long   _intr_147   # 147
    .long   _intr_148   # 148
    .long   _intr_149   # 149
    .long   _intr_150   # 150
    .long   _intr_151   # 151
    .long   _intr_152   # 152
    .long   _intr_153   # 153
    .long   _intr_154   # 154
    .long   _intr_155   # 155
    .long   _intr_156   # 156
    .long   _intr_157   # 157
    .long   _intr_158   # 158
    .long   _intr_159   # 159
    .long   _intr_160   # 160
    .long   _intr_161   # 161
    .long   _intr_162   # 162
    .long   _intr_163   # 163
    .long   _intr_164   # 164
    .long   _intr_165   # 165
    .long   _intr_166   # 166
    .long   _intr_167   # 167
    .long   _intr_168   # 168
    .long   _intr_169   # 169
    .long   _intr_170   # 170
    .long   _intr_171   # 171
    .long   _intr_172   # 172
    .long   _intr_173   # 173
    .long   _intr_174   # 174
    .long   _intr_175   # 175
    .long   _intr_176   # 176
    .long   _intr_177   # 177
    .long   _intr_178   # 178
    .long   _intr_179   # 179
    .long   _intr_180   # 180
    .long   _intr_181   # 181
    .long   _intr_182   # 182
    .long   _intr_183   # 183
    .long   _intr_184   # 184
    .long   _intr_185   # 185
    .long   _intr_186   # 186
    .long   _intr_187   # 187
    .long   _intr_188   # 188
    .long   _intr_189   # 189
    .long   _intr_190   # 190
    .long   _intr_191   # 191
    .long   _intr_192   # 192
    .long   _intr_193   # 193
    .long   _intr_194   # 194
    .long   _intr_195   # 195 0x3c3
    .long   _intr_196   # 196
    .long   _intr_197   # 197
    .long   _intr_198   # 198
    .long   _intr_199   # 199
    .long   _intr_200   # 200
    .long   _intr_201   # 201
    .long   _intr_202   # 202
    .long   _intr_203   # 203
    .long   _intr_204   # 204
    .long   _intr_205   # 205
    .long   _intr_206   # 206
    .long   _intr_207   # 207
    .long   _intr_208   # 208
    .long   _intr_209   # 209
    .long   _intr_210   # 210
    .long   _intr_211   # 211
    .long   _intr_212   # 212
    .long   _intr_213   # 213
    .long   _intr_214   # 214
    .long   _intr_215   # 215
    .long   _intr_216   # 216
    .long   _intr_217   # 217
    .long   _intr_218   # 218
    .long   _intr_219   # 219
    .long   _intr_220   # 220
    .long   _intr_221   # 221
    .long   _intr_222   # 222
    .long   _intr_223   # 223
    .long   _intr_224   # 224
    .long   _intr_225   # 225
    .long   _intr_226   # 226
    .long   _intr_227   # 227
    .long   _intr_228   # 228
    .long   _intr_229   # 229
    .long   _intr_230   # 230
    .long   _intr_231   # 231
    .long   _intr_232   # 232
    .long   _intr_233   # 233
    .long   _intr_234   # 234
    .long   _intr_235   # 235
    .long   _intr_236   # 236
    .long   _intr_237   # 237
    .long   _intr_238   # 238
    .long   _intr_239   # 239
    .long   _intr_240   # 240 0x3c0 BRI2
    .long   _intr_241   # 241 0x3c4 ERI2
    .long   _intr_242   # 242 0x3c8 RXI2
    .long   _intr_243   # 243 0x3cc TXI2
    .long   _intr_244   # 244 0x3d0 BRI3
    .long   _intr_245   # 245 0x3d4 ERI3
    .long   _intr_246   # 246 0x3d8 RXI3
    .long   _intr_247   # 247 0x3dc TXI3
    .long   _intr_248   # 248
    .long   _intr_249   # 249
    .long   _intr_250   # 250
    .long   _intr_251   # 251
    .long   _intr_252   # 252
    .long   _intr_253   # 253
    .long   _intr_254   # 254
    .long   _intr_255   # 255
    .long   _intr_256   # 256
    .long   _intr_257   # 257
    .long   _intr_258   # 258
    .long   _intr_259   # 259
    .long   _intr_260   # 260
    .long   _intr_261   # 261
    .long   _intr_262   # 262
    .long   _intr_263   # 263
    .long   _intr_264   # 264
    .long   _intr_265   # 265
    .long   _intr_266   # 266
    .long   _intr_267   # 267
    .long   _intr_268   # 268
    .long   _intr_269   # 269
    .long   _intr_270   # 270
    .long   _intr_271   # 271
    .long   _intr_272   # 272
    .long   _intr_273   # 273
    .long   _intr_274   # 274
    .long   _intr_275   # 275
    .long   _intr_276   # 276
    .long   _intr_277   # 277
    .long   _intr_278   # 278
    .long   _intr_279   # 279
    .long   _intr_280   # 280
    .long   _intr_281   # 281
    .long   _intr_282   # 282
    .long   _intr_283   # 283
    .long   _intr_284   # 284
    .long   _intr_285   # 285
    .long   _intr_286   # 286
    .long   _intr_287   # 287
    .long   _intr_288   # 288
    .long   _intr_289   # 289
    .long   _intr_290   # 290
    .long   _intr_291   # 291
    .long   _intr_292   # 292
    .long   _intr_293   # 293
    .long   _intr_294   # 294
    .long   _intr_295   # 295
    .long   _intr_296   # 296
    .long   _intr_297   # 297
    .long   _intr_298   # 298
    .long   _intr_299   # 299
    .long   _intr_300   # 300
    .long   _intr_301   # 301
    .long   _intr_302   # 302
    .long   _intr_303   # 303
    .long   _intr_304   # 304
    .long   _intr_305   # 305
    .long   _intr_306   # 306
    .long   _intr_307   # 307
    .long   _intr_308   # 308
    .long   _intr_309   # 309
    .long   _intr_310   # 310
    .long   _intr_311   # 311
    .long   _intr_312   # 312

    .section    .isr_vector
    .org    0x00000500
_startup:
	mov.l	_sp_base_addr,r15
.ifndef USE_GDBSTUB
	mov.l	_vectors_addr,r0
	ldc	r0,vbr
    mov  #0xa0,r1
    ldc r1,sr
.else
	sts.l	macl,@-r15
	sts.l	mach,@-r15
	stc.l	vbr,@-r15
	stc.l	gbr,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	r13,@-r15
	mov.l	r12,@-r15
	mov.l	r11,@-r15
	mov.l	r10,@-r15
	mov.l	r9,@-r15
	mov.l	r8,@-r15
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r4,@-r15
	mov.l	r3,@-r15
	mov.l	r2,@-r15
	mov.l	r1,@-r15
	mov.l	r0,@-r15
.endif
	mov.l	_sbss_addr,r0
	mov.l	_ebss_addr,r1
	cmp/eq	r0,r1
	bt	bss_2
	mov	#0,r2
bss_1:
	mov.l	r2,@r0
	add	#4,r0
 	cmp/hi	r0,r1
	bt	bss_1
bss_2:
.ifdef USE_GDBSTUB
	mov.l	_user_vbr_addr, r1
	mov.l	_vectors_addr, r2
	mov.l	r2, @r1
.endif
	mov.l	_start_addr, r0
	jsr	@r0
	nop
.ifdef USE_GDBSTUB
	mov.l	@r15+,r0
	mov.l	@r15+,r1
	mov.l	@r15+,r2
	mov.l	@r15+,r3
	mov.l	@r15+,r4
	mov.l	@r15+,r5
	mov.l	@r15+,r6
	mov.l	@r15+,r7
	mov.l	@r15+,r8
	mov.l	@r15+,r9
	mov.l	@r15+,r10
	mov.l	@r15+,r11
	mov.l	@r15+,r12
	mov.l	@r15+,r13
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	ldc.l	@r15+,gbr
	ldc.l	@r15+,vbr
	lds.l	@r15+,mach
	lds.l	@r15+,macl
	rts
	nop
.endif
_loop:
	bra	_loop
	nop

	.align	2
!_loop_here_addr:
!	.long	_loop_here
_vectors_addr:
	.long	_vectors
_sp_base_addr:
	.long	_stack
_start_addr:
	.long	_main
_user_vbr_addr:
	.long	0xFFF8FFFC
_sbss_addr:
	.long	_sbss
_ebss_addr:
	.long	_ebss

_intr_gen:
	# Push registers to stack except pc and sr
	sts.l	macl,@-r15
	sts.l	mach,@-r15
	stc.l	vbr,@-r15
	stc.l	gbr,@-r15
	sts.l	pr,@-r15
	mov.l	r14,@-r15
	mov.l	r13,@-r15
	mov.l	r12,@-r15
	mov.l	r11,@-r15
	mov.l	r10,@-r15
	mov.l	r9,@-r15
	mov.l	r8,@-r15
	mov.l	r7,@-r15
	mov.l	r6,@-r15
	mov.l	r5,@-r15
	mov.l	r4,@-r15
	mov.l	r3,@-r15
	mov.l	r2,@-r15
	mov.l	r1,@-r15
	mov	r0, r4
	# r1: vector no

_intr_gen_dispatch:
	mov.l	_handler_intr_addr, r1
	jsr	@r1
	nop

	mov.l	@r15+,r1
	mov.l	@r15+,r2
	mov.l	@r15+,r3
	mov.l	@r15+,r4
	mov.l	@r15+,r5
	mov.l	@r15+,r6
	mov.l	@r15+,r7
	mov.l	@r15+,r8
	mov.l	@r15+,r9
	mov.l	@r15+,r10
	mov.l	@r15+,r11
	mov.l	@r15+,r12
	mov.l	@r15+,r13
	mov.l	@r15+,r14
	lds.l	@r15+,pr
	ldc.l	@r15+,gbr
	ldc.l	@r15+,vbr
	lds.l	@r15+,mach
	lds.l	@r15+,macl
	mov.l	@r15+,r0
	rte
	nop

    .align  2
_handler_intr_addr:
    .long   _CPU_IRQInterrupt
#   .long   _handler_intr

# ===== general intr =====

_intr_004:
	mov.l	r0,@-r15
	mov	#4,r0
	bra	_intr_gen
	nop
_intr_005:
	mov.l	r0,@-r15
	mov	#5,r0
	bra	_intr_gen
	nop
_intr_006:
	mov.l	r0,@-r15
	mov	#6,r0
	bra	_intr_gen
	nop
_intr_007:
	mov.l	r0,@-r15
	mov	#7,r0
	bra	_intr_gen
	nop
_intr_009:
	mov.l	r0,@-r15
	mov	#9,r0
	bra	_intr_gen
	nop
_intr_010:
	mov.l	r0,@-r15
	mov	#10,r0
	bra	_intr_gen
	nop
_intr_011:
	mov.l	r0,@-r15
	mov	#11,r0
	bra	_intr_gen
	nop
_intr_012:
	mov.l	r0,@-r15
	mov	#12,r0
	bra	_intr_gen
	nop
_intr_013:
	mov.l	r0,@-r15
	mov	#13,r0
	bra	_intr_gen
	nop
_intr_014:
	mov.l	r0,@-r15
	mov	#14,r0
	bra	_intr_gen
	nop
_intr_015:
	mov.l	r0,@-r15
	mov	#15,r0
	bra	_intr_gen
	nop
_intr_016:
	mov.l	r0,@-r15
	mov	#16,r0
	bra	_intr_gen
	nop
_intr_017:
	mov.l	r0,@-r15
	mov	#17,r0
	bra	_intr_gen
	nop
_intr_018:
	mov.l	r0,@-r15
	mov	#18,r0
	bra	_intr_gen
	nop
_intr_019:				#
	mov.l	r0,@-r15
	movi20	#19,r0
	bra	_intr_gen
	nop
_intr_020:				#
	mov.l	r0,@-r15
	movi20	#20,r0
	bra	_intr_gen
	nop
_intr_021:				#
	mov.l	r0,@-r15
	movi20	#21,r0
	bra	_intr_gen
	nop
_intr_022:				#
	mov.l	r0,@-r15
	movi20	#22,r0
	bra	_intr_gen
	nop
_intr_023:				#
	mov.l	r0,@-r15
	movi20	#23,r0
	bra	_intr_gen
	nop
_intr_024:				#
	mov.l	r0,@-r15
	movi20	#24,r0
	bra	_intr_gen
	nop
_intr_025:				#
	mov.l	r0,@-r15
	movi20	#25,r0
	bra	_intr_gen
	nop
_intr_026:				#
	mov.l	r0,@-r15
	movi20	#26,r0
	bra	_intr_gen
	nop
_intr_027:				#
	mov.l	r0,@-r15
	movi20	#27,r0
	bra	_intr_gen
	nop
_intr_028:				#
	mov.l	r0,@-r15
	movi20	#28,r0
	bra	_intr_gen
	nop
_intr_029:				#
	mov.l	r0,@-r15
	movi20	#29,r0
	bra	_intr_gen
	nop
_intr_030:				#
	mov.l	r0,@-r15
	movi20	#30,r0
	bra	_intr_gen
	nop
_intr_031:				#
	mov.l	r0,@-r15
	movi20	#31,r0
	bra	_intr_gen
	nop
_intr_032:				# user break exception
	mov.l	r0,@-r15
	movi20	#32,r0
	bra	_intr_gen
	nop
_intr_033:				#
	mov.l	r0,@-r15
	movi20	#33,r0
	bra	_intr_gen
	nop
_intr_034:				#
	mov.l	r0,@-r15
	movi20	#34,r0
	bra	_intr_gen
	nop
_intr_035:				#
	mov.l	r0,@-r15
	movi20	#35,r0
	bra	_intr_gen
	nop
_intr_036:				#
	mov.l	r0,@-r15
	movi20	#36,r0
	bra	_intr_gen
	nop
_intr_037:				#
	mov.l	r0,@-r15
	movi20	#37,r0
	bra	_intr_gen
	nop
_intr_038:				#
	mov.l	r0,@-r15
	movi20	#38,r0
	bra	_intr_gen
	nop
_intr_039:				#
	mov.l	r0,@-r15
	movi20	#39,r0
	bra	_intr_gen
	nop
_intr_040:				#
	mov.l	r0,@-r15
	movi20	#40,r0
	bra	_intr_gen
	nop
_intr_041:				#
	mov.l	r0,@-r15
	movi20	#41,r0
	bra	_intr_gen
	nop
_intr_042:				#
	mov.l	r0,@-r15
	movi20	#42,r0
	bra	_intr_gen
	nop
_intr_043:				#
	mov.l	r0,@-r15
	movi20	#43,r0
	bra	_intr_gen
	nop
_intr_044:				#
	mov.l	r0,@-r15
	movi20	#44,r0
	bra	_intr_gen
	nop
_intr_045:				#
	mov.l	r0,@-r15
	movi20	#45,r0
	bra	_intr_gen
	nop
_intr_046:				#
	mov.l	r0,@-r15
	movi20	#46,r0
	bra	_intr_gen
	nop
_intr_047:				#
	mov.l	r0,@-r15
	movi20	#47,r0
	bra	_intr_gen
	nop
_intr_048:				#
	mov.l	r0,@-r15
	movi20	#48,r0
	bra	_intr_gen
	nop
_intr_049:				#
	mov.l	r0,@-r15
	movi20	#49,r0
	bra	_intr_gen
	nop
_intr_050:				#
	mov.l	r0,@-r15
	movi20	#50,r0
	bra	_intr_gen
	nop
_intr_051:				#
	mov.l	r0,@-r15
	movi20	#51,r0
	bra	_intr_gen
	nop
_intr_052:				#
	mov.l	r0,@-r15
	movi20	#52,r0
	bra	_intr_gen
	nop
_intr_053:				#
	mov.l	r0,@-r15
	movi20	#53,r0
	bra	_intr_gen
	nop
_intr_054:				#
	mov.l	r0,@-r15
	movi20	#54,r0
	bra	_intr_gen
	nop
_intr_055:				#
	mov.l	r0,@-r15
	movi20	#55,r0
	bra	_intr_gen
	nop
_intr_056:				#
	mov.l	r0,@-r15
	movi20	#56,r0
	bra	_intr_gen
	nop
_intr_057:				#
	mov.l	r0,@-r15
	movi20	#57,r0
	bra	_intr_gen
	nop
_intr_058:				#
	mov.l	r0,@-r15
	movi20	#58,r0
	bra	_intr_gen
	nop
_intr_059:				#
	mov.l	r0,@-r15
	movi20	#59,r0
	bra	_intr_gen
	nop
_intr_060:				#
	mov.l	r0,@-r15
	movi20	#60,r0
	bra	_intr_gen
	nop
_intr_061:				#
	mov.l	r0,@-r15
	movi20	#61,r0
	bra	_intr_gen
	nop
_intr_062:				#
	mov.l	r0,@-r15
	movi20	#62,r0
	bra	_intr_gen
	nop
_intr_063:				#
	mov.l	r0,@-r15
	movi20	#63,r0
	bra	_intr_gen
	nop
_intr_064:				#
	mov.l	r0,@-r15
	movi20	#64,r0
	bra	_intr_gen
	nop
_intr_065:				#
	mov.l	r0,@-r15
	movi20	#65,r0
	bra	_intr_gen
	nop
_intr_066:				#
	mov.l	r0,@-r15
	movi20	#66,r0
	bra	_intr_gen
	nop
_intr_067:				#
	mov.l	r0,@-r15
	movi20	#67,r0
	bra	_intr_gen
	nop
_intr_068:				#
	mov.l	r0,@-r15
	movi20	#68,r0
	bra	_intr_gen
	nop
_intr_069:				#
	mov.l	r0,@-r15
	movi20	#69,r0
	bra	_intr_gen
	nop
_intr_070:				#
	mov.l	r0,@-r15
	movi20	#70,r0
	bra	_intr_gen
	nop
_intr_071:				#
	mov.l	r0,@-r15
	movi20	#71,r0
	bra	_intr_gen
	nop
_intr_072:				#
	mov.l	r0,@-r15
	movi20	#72,r0
	bra	_intr_gen
	nop
_intr_073:				#
	mov.l	r0,@-r15
	movi20	#73,r0
	bra	_intr_gen
	nop
_intr_074:				#
	mov.l	r0,@-r15
	movi20	#74,r0
	bra	_intr_gen
	nop
_intr_075:				#
	mov.l	r0,@-r15
	movi20	#75,r0
	bra	_intr_gen
	nop
_intr_076:				#
	mov.l	r0,@-r15
	movi20	#76,r0
	bra	_intr_gen
	nop
_intr_077:				#
	mov.l	r0,@-r15
	movi20	#77,r0
	bra	_intr_gen
	nop
_intr_078:				#
	mov.l	r0,@-r15
	movi20	#78,r0
	bra	_intr_gen
	nop
_intr_079:				#
	mov.l	r0,@-r15
	movi20	#79,r0
	bra	_intr_gen
	nop
_intr_080:				#
	mov.l	r0,@-r15
	movi20	#80,r0
	bra	_intr_gen
	nop
_intr_081:				#
	mov.l	r0,@-r15
	movi20	#81,r0
	bra	_intr_gen
	nop
_intr_082:				#
	mov.l	r0,@-r15
	movi20	#82,r0
	bra	_intr_gen
	nop
_intr_083:				#
	mov.l	r0,@-r15
	movi20	#83,r0
	bra	_intr_gen
	nop
_intr_084:				#
	mov.l	r0,@-r15
	movi20	#84,r0
	bra	_intr_gen
	nop
_intr_085:				#
	mov.l	r0,@-r15
	movi20	#85,r0
	bra	_intr_gen
	nop
_intr_086:				#
	mov.l	r0,@-r15
	movi20	#86,r0
	bra	_intr_gen
	nop
_intr_087:				#
	mov.l	r0,@-r15
	movi20	#87,r0
	bra	_intr_gen
	nop
_intr_088:				#
	mov.l	r0,@-r15
	movi20	#88,r0
	bra	_intr_gen
	nop
_intr_089:				#
	mov.l	r0,@-r15
	movi20	#89,r0
	bra	_intr_gen
	nop
_intr_090:				#
	mov.l	r0,@-r15
	movi20	#90,r0
	bra	_intr_gen
	nop
_intr_091:				#
	mov.l	r0,@-r15
	movi20	#91,r0
	bra	_intr_gen
	nop
_intr_092:				#
	mov.l	r0,@-r15
	movi20	#92,r0
	bra	_intr_gen
	nop
_intr_093:				#
	mov.l	r0,@-r15
	movi20	#93,r0
	bra	_intr_gen
	nop
_intr_094:				#
	mov.l	r0,@-r15
	movi20	#94,r0
	bra	_intr_gen
	nop
_intr_095:				#
	mov.l	r0,@-r15
	movi20	#95,r0
	bra	_intr_gen
	nop
_intr_096:				#
	mov.l	r0,@-r15
	movi20	#96,r0
	bra	_intr_gen
	nop
_intr_097:				#
	mov.l	r0,@-r15
	movi20	#97,r0
	bra	_intr_gen
	nop
_intr_098:				#
	mov.l	r0,@-r15
	movi20	#98,r0
	bra	_intr_gen
	nop
_intr_099:				#
	mov.l	r0,@-r15
	movi20	#99,r0
	bra	_intr_gen
	nop
_intr_100:				#
	mov.l	r0,@-r15
	movi20	#100,r0
	bra	_intr_gen
	nop
_intr_101:				#
	mov.l	r0,@-r15
	movi20	#101,r0
	bra	_intr_gen
	nop
_intr_102:				#
	mov.l	r0,@-r15
	movi20	#102,r0
	bra	_intr_gen
	nop
_intr_103:				#
	mov.l	r0,@-r15
	movi20	#103,r0
	bra	_intr_gen
	nop
_intr_104:				#
	mov.l	r0,@-r15
	movi20	#104,r0
	bra	_intr_gen
	nop
_intr_105:				#
	mov.l	r0,@-r15
	movi20	#105,r0
	bra	_intr_gen
	nop
_intr_106:				#
	mov.l	r0,@-r15
	movi20	#106,r0
	bra	_intr_gen
	nop
_intr_107:				#
	mov.l	r0,@-r15
	movi20	#107,r0
	bra	_intr_gen
	nop
_intr_108:				#
	mov.l	r0,@-r15
	movi20	#108,r0
	bra	_intr_gen
	nop
_intr_109:				#
	mov.l	r0,@-r15
	movi20	#109,r0
	bra	_intr_gen
	nop
_intr_110:				#
	mov.l	r0,@-r15
	movi20	#110,r0
	bra	_intr_gen
	nop
_intr_111:				#
	mov.l	r0,@-r15
	movi20	#111,r0
	bra	_intr_gen
	nop
_intr_112:				#
	mov.l	r0,@-r15
	movi20	#112,r0
	bra	_intr_gen
	nop
_intr_113:				#
	mov.l	r0,@-r15
	movi20	#113,r0
	bra	_intr_gen
	nop
_intr_114:				#
	mov.l	r0,@-r15
	movi20	#114,r0
	bra	_intr_gen
	nop
_intr_115:				#
	mov.l	r0,@-r15
	movi20	#115,r0
	bra	_intr_gen
	nop
_intr_116:				#
	mov.l	r0,@-r15
	movi20	#116,r0
	bra	_intr_gen
	nop
_intr_117:				#
	mov.l	r0,@-r15
	movi20	#117,r0
	bra	_intr_gen
	nop
_intr_118:				#
	mov.l	r0,@-r15
	movi20	#118,r0
	bra	_intr_gen
	nop
_intr_119:				#
	mov.l	r0,@-r15
	movi20	#119,r0
	bra	_intr_gen
	nop
_intr_120:				#
	mov.l	r0,@-r15
	movi20	#120,r0
	bra	_intr_gen
	nop
_intr_121:				#
	mov.l	r0,@-r15
	movi20	#121,r0
	bra	_intr_gen
	nop
_intr_122:				#
	mov.l	r0,@-r15
	movi20	#122,r0
	bra	_intr_gen
	nop
_intr_123:				#
	mov.l	r0,@-r15
	movi20	#123,r0
	bra	_intr_gen
	nop
_intr_124:				#
	mov.l	r0,@-r15
	movi20	#124,r0
	bra	_intr_gen
	nop
_intr_125:				#
	mov.l	r0,@-r15
	movi20	#125,r0
	bra	_intr_gen
	nop
_intr_126:				#
	mov.l	r0,@-r15
	movi20	#126,r0
	bra	_intr_gen
	nop
_intr_127:				#
	mov.l	r0,@-r15
	movi20	#127,r0
	bra	_intr_gen
	nop
_intr_128:				#
	mov.l	r0,@-r15
	movi20	#128,r0
	bra	_intr_gen
	nop
_intr_129:				#
	mov.l	r0,@-r15
	movi20	#129,r0
	bra	_intr_gen
	nop
_intr_130:				#
	mov.l	r0,@-r15
	movi20	#130,r0
	bra	_intr_gen
	nop
_intr_131:				#
	mov.l	r0,@-r15
	movi20	#131,r0
	bra	_intr_gen
	nop
_intr_132:				#
	mov.l	r0,@-r15
	movi20	#132,r0
	bra	_intr_gen
	nop
_intr_133:				#
	mov.l	r0,@-r15
	movi20	#133,r0
	bra	_intr_gen
	nop
_intr_134:				#
	mov.l	r0,@-r15
	movi20	#134,r0
	bra	_intr_gen
	nop
_intr_135:				#
	mov.l	r0,@-r15
	movi20	#135,r0
	bra	_intr_gen
	nop
_intr_136:				#
	mov.l	r0,@-r15
	movi20	#136,r0
	bra	_intr_gen
	nop
_intr_137:				#
	mov.l	r0,@-r15
	movi20	#137,r0
	bra	_intr_gen
	nop
_intr_138:				#
	mov.l	r0,@-r15
	movi20	#138,r0
	bra	_intr_gen
	nop
_intr_139:				#
	mov.l	r0,@-r15
	movi20	#139,r0
	bra	_intr_gen
	nop
_intr_140:				#
	mov.l	r0,@-r15
	movi20	#140,r0
	bra	_intr_gen
	nop
_intr_141:				#
	mov.l	r0,@-r15
	movi20	#141,r0
	bra	_intr_gen
	nop
_intr_142:				#
	mov.l	r0,@-r15
	movi20	#142,r0
	bra	_intr_gen
	nop
_intr_143:				#
	mov.l	r0,@-r15
	movi20	#143,r0
	bra	_intr_gen
	nop
_intr_144:				#
	mov.l	r0,@-r15
	movi20	#144,r0
	bra	_intr_gen
	nop
_intr_145:				#
	mov.l	r0,@-r15
	movi20	#145,r0
	bra	_intr_gen
	nop
_intr_146:				#
	mov.l	r0,@-r15
	movi20	#146,r0
	bra	_intr_gen
	nop
_intr_147:				#
	mov.l	r0,@-r15
	movi20	#147,r0
	bra	_intr_gen
	nop
_intr_148:				#
	mov.l	r0,@-r15
	movi20	#148,r0
	bra	_intr_gen
	nop
_intr_149:				#
	mov.l	r0,@-r15
	movi20	#149,r0
	bra	_intr_gen
	nop
_intr_150:				#
	mov.l	r0,@-r15
	movi20	#150,r0
	bra	_intr_gen
	nop
_intr_151:				#
	mov.l	r0,@-r15
	movi20	#151,r0
	bra	_intr_gen
	nop
_intr_152:				#
	mov.l	r0,@-r15
	movi20	#152,r0
	bra	_intr_gen
	nop
_intr_153:				#
	mov.l	r0,@-r15
	movi20	#153,r0
	bra	_intr_gen
	nop
_intr_154:				#
	mov.l	r0,@-r15
	movi20	#154,r0
	bra	_intr_gen
	nop
_intr_155:				#
	mov.l	r0,@-r15
	movi20	#155,r0
	bra	_intr_gen
	nop
_intr_156:				#
	mov.l	r0,@-r15
	movi20	#156,r0
	bra	_intr_gen
	nop
_intr_157:				#
	mov.l	r0,@-r15
	movi20	#157,r0
	bra	_intr_gen
	nop
_intr_158:				#
	mov.l	r0,@-r15
	movi20	#158,r0
	bra	_intr_gen
	nop
_intr_159:				#
	mov.l	r0,@-r15
	movi20	#159,r0
	bra	_intr_gen
	nop
_intr_160:				#
	mov.l	r0,@-r15
	movi20	#160,r0
	bra	_intr_gen
	nop
_intr_161:				#
	mov.l	r0,@-r15
	movi20	#161,r0
	bra	_intr_gen
	nop
_intr_162:				#
	mov.l	r0,@-r15
	movi20	#162,r0
	bra	_intr_gen
	nop
_intr_163:				#
	mov.l	r0,@-r15
	movi20	#163,r0
	bra	_intr_gen
	nop
_intr_164:				#
	mov.l	r0,@-r15
	movi20	#164,r0
	bra	_intr_gen
	nop
_intr_165:				#
	mov.l	r0,@-r15
	movi20	#165,r0
	bra	_intr_gen
	nop
_intr_166:				#
	mov.l	r0,@-r15
	movi20	#166,r0
	bra	_intr_gen
	nop
_intr_167:				#
	mov.l	r0,@-r15
	movi20	#167,r0
	bra	_intr_gen
	nop
_intr_168:				#
	mov.l	r0,@-r15
	movi20	#168,r0
	bra	_intr_gen
	nop
_intr_169:				#
	mov.l	r0,@-r15
	movi20	#169,r0
	bra	_intr_gen
	nop
_intr_170:				#
	mov.l	r0,@-r15
	movi20	#170,r0
	bra	_intr_gen
	nop
_intr_171:				#
	mov.l	r0,@-r15
	movi20	#171,r0
	bra	_intr_gen
	nop
_intr_172:				#
	mov.l	r0,@-r15
	movi20	#172,r0
	bra	_intr_gen
	nop
_intr_173:				#
	mov.l	r0,@-r15
	movi20	#173,r0
	bra	_intr_gen
	nop
_intr_174:				#
	mov.l	r0,@-r15
	movi20	#174,r0
	bra	_intr_gen
	nop
_intr_175:				#
	mov.l	r0,@-r15
	movi20	#175,r0
	bra	_intr_gen
	nop
_intr_176:				#
	mov.l	r0,@-r15
	movi20	#176,r0
	bra	_intr_gen
	nop
_intr_177:				#
	mov.l	r0,@-r15
	movi20	#177,r0
	bra	_intr_gen
	nop
_intr_178:				#
	mov.l	r0,@-r15
	movi20	#178,r0
	bra	_intr_gen
	nop
_intr_179:				#
	mov.l	r0,@-r15
	movi20	#179,r0
	bra	_intr_gen
	nop
_intr_180:				#
	mov.l	r0,@-r15
	movi20	#180,r0
	bra	_intr_gen
	nop
_intr_181:				#
	mov.l	r0,@-r15
	movi20	#181,r0
	bra	_intr_gen
	nop
_intr_182:				#
	mov.l	r0,@-r15
	movi20	#182,r0
	bra	_intr_gen
	nop
_intr_183:				#
	mov.l	r0,@-r15
	movi20	#183,r0
	bra	_intr_gen
	nop
_intr_184:				#
	mov.l	r0,@-r15
	movi20	#184,r0
	bra	_intr_gen
	nop
_intr_185:				#
	mov.l	r0,@-r15
	movi20	#185,r0
	bra	_intr_gen
	nop
_intr_186:				#
	mov.l	r0,@-r15
	movi20	#186,r0
	bra	_intr_gen
	nop
_intr_187:				#
	mov.l	r0,@-r15
	movi20	#187,r0
	bra	_intr_gen
	nop
_intr_188:				#
	mov.l	r0,@-r15
	movi20	#188,r0
	bra	_intr_gen
	nop
_intr_189:				#
	mov.l	r0,@-r15
	movi20	#189,r0
	bra	_intr_gen
	nop
_intr_190:				#
	mov.l	r0,@-r15
	movi20	#190,r0
	bra	_intr_gen
	nop
_intr_191:				#
	mov.l	r0,@-r15
	movi20	#191,r0
	bra	_intr_gen
	nop
_intr_192:				#
	mov.l	r0,@-r15
	movi20	#192,r0
	bra	_intr_gen
	nop
_intr_193:				#
	mov.l	r0,@-r15
	movi20	#193,r0
	bra	_intr_gen
	nop
_intr_194:				#
	mov.l	r0,@-r15
	movi20	#194,r0
	bra	_intr_gen
	nop
_intr_195:				# break exception
	mov.l	r0,@-r15
	movi20	#195,r0
	bra	_intr_gen
	nop
_intr_196:				#
	mov.l	r0,@-r15
	movi20	#196,r0
	bra	_intr_gen
	nop
_intr_197:				#
	mov.l	r0,@-r15
	movi20	#197,r0
	bra	_intr_gen
	nop
_intr_198:				#
	mov.l	r0,@-r15
	movi20	#198,r0
	bra	_intr_gen
	nop
_intr_199:				#
	mov.l	r0,@-r15
	movi20	#199,r0
	bra	_intr_gen
	nop
_intr_200:				#
	mov.l	r0,@-r15
	movi20	#200,r0
	bra	_intr_gen
	nop
_intr_201:				#
	mov.l	r0,@-r15
	movi20	#201,r0
	bra	_intr_gen
	nop
_intr_202:				#
	mov.l	r0,@-r15
	movi20	#202,r0
	bra	_intr_gen
	nop
_intr_203:				#
	mov.l	r0,@-r15
	movi20	#203,r0
	bra	_intr_gen
	nop
_intr_204:				#
	mov.l	r0,@-r15
	movi20	#204,r0
	bra	_intr_gen
	nop
_intr_205:				#
	mov.l	r0,@-r15
	movi20	#205,r0
	bra	_intr_gen
	nop
_intr_206:				#
	mov.l	r0,@-r15
	movi20	#206,r0
	bra	_intr_gen
	nop
_intr_207:				#
	mov.l	r0,@-r15
	movi20	#207,r0
	bra	_intr_gen
	nop
_intr_208:				#
	mov.l	r0,@-r15
	movi20	#208,r0
	bra	_intr_gen
	nop
_intr_209:				#
	mov.l	r0,@-r15
	movi20	#209,r0
	bra	_intr_gen
	nop
_intr_210:				#
	mov.l	r0,@-r15
	movi20	#210,r0
	bra	_intr_gen
	nop
_intr_211:				#
	mov.l	r0,@-r15
	movi20	#211,r0
	bra	_intr_gen
	nop
_intr_212:				#
	mov.l	r0,@-r15
	movi20	#212,r0
	bra	_intr_gen
	nop
_intr_213:				#
	mov.l	r0,@-r15
	movi20	#213,r0
	bra	_intr_gen
	nop
_intr_214:				#
	mov.l	r0,@-r15
	movi20	#214,r0
	bra	_intr_gen
	nop
_intr_215:				#
	mov.l	r0,@-r15
	movi20	#215,r0
	bra	_intr_gen
	nop
_intr_216:				#
	mov.l	r0,@-r15
	movi20	#216,r0
	bra	_intr_gen
	nop
_intr_217:				#
	mov.l	r0,@-r15
	movi20	#217,r0
	bra	_intr_gen
	nop
_intr_218:				#
	mov.l	r0,@-r15
	movi20	#218,r0
	bra	_intr_gen
	nop
_intr_219:				#
	mov.l	r0,@-r15
	movi20	#219,r0
	bra	_intr_gen
	nop
_intr_220:				#
	mov.l	r0,@-r15
	movi20	#220,r0
	bra	_intr_gen
	nop
_intr_221:				#
	mov.l	r0,@-r15
	movi20	#221,r0
	bra	_intr_gen
	nop
_intr_222:				#
	mov.l	r0,@-r15
	movi20	#222,r0
	bra	_intr_gen
	nop
_intr_223:				#
	mov.l	r0,@-r15
	movi20	#223,r0
	bra	_intr_gen
	nop
_intr_224:				#
	mov.l	r0,@-r15
	movi20	#224,r0
	bra	_intr_gen
	nop
_intr_225:				#
	mov.l	r0,@-r15
	movi20	#225,r0
	bra	_intr_gen
	nop
_intr_226:				#
	mov.l	r0,@-r15
	movi20	#226,r0
	bra	_intr_gen
	nop
_intr_227:				#
	mov.l	r0,@-r15
	movi20	#227,r0
	bra	_intr_gen
	nop
_intr_228:				#
	mov.l	r0,@-r15
	movi20	#228,r0
	bra	_intr_gen
	nop
_intr_229:				#
	mov.l	r0,@-r15
	movi20	#229,r0
	bra	_intr_gen
	nop
_intr_230:				#
	mov.l	r0,@-r15
	movi20	#230,r0
	bra	_intr_gen
	nop
_intr_231:				#
	mov.l	r0,@-r15
	movi20	#231,r0
	bra	_intr_gen
	nop
_intr_232:				#
	mov.l	r0,@-r15
	movi20	#232,r0
	bra	_intr_gen
	nop
_intr_233:				#
	mov.l	r0,@-r15
	movi20	#233,r0
	bra	_intr_gen
	nop
_intr_234:				#
	mov.l	r0,@-r15
	movi20	#234,r0
	bra	_intr_gen
	nop
_intr_235:				#
	mov.l	r0,@-r15
	movi20	#235,r0
	bra	_intr_gen
	nop
_intr_236:				#
	mov.l	r0,@-r15
	movi20	#236,r0
	bra	_intr_gen
	nop
_intr_237:				#
	mov.l	r0,@-r15
	movi20	#237,r0
	bra	_intr_gen
	nop
_intr_238:				#
	mov.l	r0,@-r15
	movi20	#238,r0
	bra	_intr_gen
	nop
_intr_239:				#
	mov.l	r0,@-r15
	movi20	#239,r0
	bra	_intr_gen
	nop
_intr_240:				#
	mov.l	r0,@-r15
	movi20	#240,r0
	bra	_intr_gen
	nop
_intr_241:				#
	mov.l	r0,@-r15
	movi20	#241,r0
	bra	_intr_gen
	nop
_intr_242:				# serial ch 2 rx
	mov.l	r0,@-r15
	movi20	#242,r0
	bra	_intr_gen
	nop
_intr_243:				#
	mov.l	r0,@-r15
	movi20	#243,r0
	bra	_intr_gen
	nop
_intr_244:				#
	mov.l	r0,@-r15
	movi20	#244,r0
	bra	_intr_gen
	nop
_intr_245:				#
	mov.l	r0,@-r15
	movi20	#245,r0
	bra	_intr_gen
	nop
_intr_246:				#
	mov.l	r0,@-r15
	movi20	#246,r0
	bra	_intr_gen
	nop
_intr_247:				#
	mov.l	r0,@-r15
	movi20	#247,r0
	bra	_intr_gen
	nop
_intr_248:				#
	mov.l	r0,@-r15
	movi20	#248,r0
	bra	_intr_gen
	nop
_intr_249:				#
	mov.l	r0,@-r15
	movi20	#249,r0
	bra	_intr_gen
	nop
_intr_250:				#
	mov.l	r0,@-r15
	movi20	#250,r0
	bra	_intr_gen
	nop
_intr_251:				#
	mov.l	r0,@-r15
	movi20	#251,r0
	bra	_intr_gen
	nop
_intr_252:				#
	mov.l	r0,@-r15
	movi20	#252,r0
	bra	_intr_gen
	nop
_intr_253:				#
	mov.l	r0,@-r15
	movi20	#253,r0
	bra	_intr_gen
	nop
_intr_254:				#
	mov.l	r0,@-r15
	movi20	#254,r0
	bra	_intr_gen
	nop
_intr_255:				#
	mov.l	r0,@-r15
	movi20	#255,r0
	bra	_intr_gen
	nop
_intr_256:				#
	mov.l	r0,@-r15
	movi20	#256,r0
	bra	_intr_gen
	nop
_intr_257:				#
	mov.l	r0,@-r15
	movi20	#257,r0
	bra	_intr_gen
	nop
_intr_258:				#
	mov.l	r0,@-r15
	movi20	#258,r0
	bra	_intr_gen
	nop
_intr_259:				#
	mov.l	r0,@-r15
	movi20	#259,r0
	bra	_intr_gen
	nop
_intr_260:				#
	mov.l	r0,@-r15
	movi20	#260,r0
	bra	_intr_gen
	nop
_intr_261:				#
	mov.l	r0,@-r15
	movi20	#261,r0
	bra	_intr_gen
	nop
_intr_262:				#
	mov.l	r0,@-r15
	movi20	#262,r0
	bra	_intr_gen
	nop
_intr_263:				#
	mov.l	r0,@-r15
	movi20	#263,r0
	bra	_intr_gen
	nop
_intr_264:				#
	mov.l	r0,@-r15
	movi20	#264,r0
	bra	_intr_gen
	nop
_intr_265:				#
	mov.l	r0,@-r15
	movi20	#265,r0
	bra	_intr_gen
	nop
_intr_266:				#
	mov.l	r0,@-r15
	movi20	#266,r0
	bra	_intr_gen
	nop
_intr_267:				#
	mov.l	r0,@-r15
	movi20	#267,r0
	bra	_intr_gen
	nop
_intr_268:				#
	mov.l	r0,@-r15
	movi20	#268,r0
	bra	_intr_gen
	nop
_intr_269:				#
	mov.l	r0,@-r15
	movi20	#269,r0
	bra	_intr_gen
	nop
_intr_270:				#
	mov.l	r0,@-r15
	movi20	#270,r0
	bra	_intr_gen
	nop
_intr_271:				#
	mov.l	r0,@-r15
	movi20	#271,r0
	bra	_intr_gen
	nop
_intr_272:				#
	mov.l	r0,@-r15
	movi20	#272,r0
	bra	_intr_gen
	nop
_intr_273:				#
	mov.l	r0,@-r15
	movi20	#273,r0
	bra	_intr_gen
	nop
_intr_274:				#
	mov.l	r0,@-r15
	movi20	#274,r0
	bra	_intr_gen
	nop
_intr_275:				#
	mov.l	r0,@-r15
	movi20	#275,r0
	bra	_intr_gen
	nop
_intr_276:				#
	mov.l	r0,@-r15
	movi20	#276,r0
	bra	_intr_gen
	nop
_intr_277:				#
	mov.l	r0,@-r15
	movi20	#277,r0
	bra	_intr_gen
	nop
_intr_278:				#
	mov.l	r0,@-r15
	movi20	#278,r0
	bra	_intr_gen
	nop
_intr_279:				#
	mov.l	r0,@-r15
	movi20	#279,r0
	bra	_intr_gen
	nop
_intr_280:				#
	mov.l	r0,@-r15
	movi20	#280,r0
	bra	_intr_gen
	nop
_intr_281:				#
	mov.l	r0,@-r15
	movi20	#281,r0
	bra	_intr_gen
	nop
_intr_282:				#
	mov.l	r0,@-r15
	movi20	#282,r0
	bra	_intr_gen
	nop
_intr_283:				#
	mov.l	r0,@-r15
	movi20	#283,r0
	bra	_intr_gen
	nop
_intr_284:				#
	mov.l	r0,@-r15
	movi20	#284,r0
	bra	_intr_gen
	nop
_intr_285:				#
	mov.l	r0,@-r15
	movi20	#285,r0
	bra	_intr_gen
	nop
_intr_286:				#
	mov.l	r0,@-r15
	movi20	#286,r0
	bra	_intr_gen
	nop
_intr_287:				#
	mov.l	r0,@-r15
	movi20	#287,r0
	bra	_intr_gen
	nop
_intr_288:				#
	mov.l	r0,@-r15
	movi20	#288,r0
	bra	_intr_gen
	nop
_intr_289:				#
	mov.l	r0,@-r15
	movi20	#289,r0
	bra	_intr_gen
	nop
_intr_290:				#
	mov.l	r0,@-r15
	movi20	#290,r0
	bra	_intr_gen
	nop
_intr_291:				#
	mov.l	r0,@-r15
	movi20	#291,r0
	bra	_intr_gen
	nop
_intr_292:				#
	mov.l	r0,@-r15
	movi20	#292,r0
	bra	_intr_gen
	nop
_intr_293:				#
	mov.l	r0,@-r15
	movi20	#293,r0
	bra	_intr_gen
	nop
_intr_294:				#
	mov.l	r0,@-r15
	movi20	#294,r0
	bra	_intr_gen
	nop
_intr_295:				#
	mov.l	r0,@-r15
	movi20	#295,r0
	bra	_intr_gen
	nop
_intr_296:				#
	mov.l	r0,@-r15
	movi20	#296,r0
	bra	_intr_gen
	nop
_intr_297:				#
	mov.l	r0,@-r15
	movi20	#297,r0
	bra	_intr_gen
	nop
_intr_298:				#
	mov.l	r0,@-r15
	movi20	#298,r0
	bra	_intr_gen
	nop
_intr_299:				#
	mov.l	r0,@-r15
	movi20	#299,r0
	bra	_intr_gen
	nop
_intr_300:				#
	mov.l	r0,@-r15
	movi20	#300,r0
	bra	_intr_gen
	nop
_intr_301:				#
	mov.l	r0,@-r15
	movi20	#301,r0
	bra	_intr_gen
	nop
_intr_302:				#
	mov.l	r0,@-r15
	movi20	#302,r0
	bra	_intr_gen
	nop
_intr_303:				#
	mov.l	r0,@-r15
	movi20	#303,r0
	bra	_intr_gen
	nop
_intr_304:				#
	mov.l	r0,@-r15
	movi20	#304,r0
	bra	_intr_gen
	nop
_intr_305:				#
	mov.l	r0,@-r15
	movi20	#305,r0
	bra	_intr_gen
	nop
_intr_306:				#
	mov.l	r0,@-r15
	movi20	#306,r0
	bra	_intr_gen
	nop
_intr_307:				#
	mov.l	r0,@-r15
	movi20	#307,r0
	bra	_intr_gen
	nop
_intr_308:				#
	mov.l	r0,@-r15
	movi20	#308,r0
	bra	_intr_gen
	nop
_intr_309:				#
	mov.l	r0,@-r15
	movi20	#309,r0
	bra	_intr_gen
	nop
_intr_310:				#
	mov.l	r0,@-r15
	movi20	#310,r0
	bra	_intr_gen
	nop
_intr_311:				#
	mov.l	r0,@-r15
	movi20	#311,r0
	bra	_intr_gen
	nop
_intr_312:				#
	mov.l	r0,@-r15
	movi20	#312,r0
	bra	_intr_gen
	nop

    .end
