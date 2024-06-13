/* Authors: Lutong Wang and Bangqi Xu */
/*
 * Copyright (c) 2019, The Regents of the University of California
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <iostream>
#include <memory>
#include <string>
#include "frDesign.h"
#include "db/obj/frBlock.h"

//下边的变量来自于global.cpp,其值来自于main.cpp中用户的输入文件
extern std::string DEF_FILE;
extern std::string GUIDE_FILE;
extern std::string OUTGUIDE_FILE;
extern std::string LEF_FILE;
extern std::string OUTTA_FILE;
extern std::string OUT_FILE;
extern std::string REF_OUT_FILE;
extern std::string DBPROCESSNODE;
extern std::string OUT_MAZE_FILE;
extern std::string DRC_RPT_FILE;
extern std::string CMAP_FILE;
// to be removed
extern int OR_SEED; //在global.cpp中初始化为-1
extern double OR_K; //在global.cpp中初始化为0

extern int MAX_THREADS ;//在global.cpp中初始化为1
extern int BATCHSIZE ;  //在global.cpp中初始化为1024
extern int BATCHSIZETA; //在global.cpp中初始化为8
extern int MTSAFEDIST ; //在global.cpp中初始化为2000
extern int DRCSAFEDIST ;//在global.cpp中初始化为500
extern int VERBOSE     ;//在global.cpp中初始化为1
extern int BOTTOM_ROUTING_LAYER;//在global.cpp中初始化为2
//在io_parser_helper.cpp的io::Parser::postProcess()函数中被设置为18
extern int TOP_ROUTING_LAYER;//在global.cpp中初始化为std::numeric_limits<frLayerNum>::max()
extern bool ALLOW_PIN_AS_FEEDTHROUGH;//在global.cpp中初始化为false
extern bool USENONPREFTRACKS;//在global.cpp中初始化为true
extern bool USEMINSPACING_OBS;//在global.cpp中初始化为true
extern bool RESERVE_VIA_ACCESS;//在global.cpp中初始化为true
extern bool ENABLE_BOUNDARY_MAR_FIX;//在global.cpp中初始化为true
extern bool ENABLE_VIA_GEN;//在global.cpp中初始化为true
//extern int TEST;
extern fr::frLayerNum VIAINPIN_BOTTOMLAYERNUM;  //2
extern fr::frLayerNum VIAINPIN_TOPLAYERNUM;     //2
extern fr::frLayerNum VIAONLY_MACROCELLPIN_BOTTOMLAYERNUM;//2
extern fr::frLayerNum VIAONLY_MACROCELLPIN_TOPLAYERNUM;//2
extern fr::frLayerNum VIAONLY_STDCELLPIN_BOTTOMLAYERNUM;//2
extern fr::frLayerNum VIAONLY_STDCELLPIN_TOPLAYERNUM;//2
// uni-directional routing related - 用户输入
extern fr::frLayerNum ONGRIDONLY_WIRE_PREF_BOTTOMLAYERNUM;  //用户输入 默认int_max
extern fr::frLayerNum ONGRIDONLY_WIRE_PREF_TOPLAYERNUM;     //用户输入 默认int_min
extern fr::frLayerNum ONGRIDONLY_WIRE_NONPREF_BOTTOMLAYERNUM; //用户输入 默认int_max
extern fr::frLayerNum ONGRIDONLY_WIRE_NONPREF_TOPLAYERNUM;  //用户输入 默认int_min
extern fr::frLayerNum ONGRIDONLY_VIA_BOTTOMLAYERNUM;        //用户输入 默认int_max
extern fr::frLayerNum ONGRIDONLY_VIA_TOPLAYERNUM;           //用户输入 默认int_min

extern fr::frLayerNum VIA_ACCESS_LAYERNUM;  //2

extern int MINNUMACCESSPOINT_MACROCELLPIN;  //3
extern int MINNUMACCESSPOINT_STDCELLPIN;    //3
extern int ACCESS_PATTERN_END_ITERATION_NUM;//5

extern int END_ITERATION; //在global.cpp中初始化为80

extern fr::frUInt4 TAVIACOST; //1
extern fr::frUInt4 TAPINCOST; //4
extern fr::frUInt4 TAALIGNCOST;//4
extern fr::frUInt4 TADRCCOST; //32
extern float       TASHAPEBLOATWIDTH;//1.5

extern fr::frUInt4 VIACOST; //4

extern fr::frUInt4 GRIDCOST;//2
extern fr::frUInt4 SHAPECOST;//8
extern fr::frUInt4 DRCCOST;//8
extern fr::frUInt4 MARKERCOST;//32
extern fr::frUInt4 MARKERBLOATWIDTH;//1
extern fr::frUInt4 BLOCKCOST;//32
extern fr::frUInt4 GUIDECOST;//1
extern float       MARKERDECAY;//0.8
extern float       SHAPEBLOATWIDTH;//3
extern int MISALIGNMENTCOST;//8

// GR
extern int HISTCOST;  //32
extern int CONGCOST;  //8

#define DIRBITSIZE 3  //3 - 可能表示每个方向上可以表示的位数，用于编码方向信息
#define WAVEFRONTBUFFERSIZE 2 //2 - 能表示波前缓冲区的大小或者是与波前相关的某种形式的“窗口”大小
#define WAVEFRONTBITSIZE (WAVEFRONTBUFFERSIZE * DIRBITSIZE) //(WAVEFRONTBUFFERSIZE*3)
//111是一个八进制数，相当于001 001 001
//相当于把八进制数111左移3((2-1)*3)位 = 001 001 001 000 000 000(二进制)  00700000(十六进制)
#define WAVEFRONTBUFFERHIGHMASK (111 << ((WAVEFRONTBUFFERSIZE - 1) * DIRBITSIZE))

// GR
#define GRWAVEFRONTBUFFERSIZE 2 //2 - 可能表示波前缓冲区中的单元格数量或者缓冲区的大小
//计算每个波前缓冲单元所需的位数。如果 DIRBITSIZE 是定义方向的位数，则 GRWAVEFRONTBITSIZE 将给出每个单元格在位图中所需的位数
#define GRWAVEFRONTBITSIZE (GRWAVEFRONTBUFFERSIZE * DIRBITSIZE)
//定义了一个掩码，用于访问或修改波前缓冲区中的高位,同上边的 WAVEFRONTBUFFERHIGHMASK
#define GRWAVEFRONTBUFFERHIGHMASK (111 << ((GRWAVEFRONTBUFFERSIZE - 1) * DIRBITSIZE))


namespace fr {
  extern frCoord getGCELLGRIDX(); //可能返回一个与全局单元格（GCELL）网格相关的 X 方向上的坐标或尺寸
  extern frCoord getGCELLGRIDY(); //返回 Y 方向上的网格尺寸或坐标
  extern frCoord getGCELLOFFSETX(); //可能返回全局单元格在 X 方向上的偏移量，用于确定单元格的确切位置
  extern frCoord getGCELLOFFSETY(); //返回 Y 方向上的偏移量
}

//运算符重载部分
extern std::ostream& operator<< (std::ostream& os, const fr::frViaDef &viaDefIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frBlock &blockIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frInst &instIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frInstTerm &instTermIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frTerm &termIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frPin &pinIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frRect &pinFig);
extern std::ostream& operator<< (std::ostream& os, const fr::frPolygon &pinFig);
extern std::ostream& operator<< (std::ostream& os, const fr::frNet &net);
//extern std::ostream& operator<< (std::ostream& os, const fr::frLayerBlockage &blk);

extern std::ostream& operator<< (std::ostream& os, const fr::frPoint &pIn);
extern std::ostream& operator<< (std::ostream& os, const fr::frBox &box);

//extern size_t getPeakRSS();
//extern size_t getCurrentRSS();

//extern void printAllMacros(const std::shared_ptr<fr::frDesign> &design);

#endif
