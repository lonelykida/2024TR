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

#include <iostream>
#include "global.h"

using namespace std;
using namespace fr;

//下边的参数都在main.cpp中用到，是从命令行参数中读取的
string DEF_FILE;
string GUIDE_FILE;
string OUTGUIDE_FILE;
string LEF_FILE;
string OUTTA_FILE;
string OUT_FILE;
string REF_OUT_FILE;
string OUT_MAZE_FILE;
string DRC_RPT_FILE;
string CMAP_FILE;
// to be removed
int OR_SEED = -1;
double OR_K = 0;

//string DBPROCESSNODE = "N16_11m_2xa1xd3xe2y2r_utrdl";
string DBPROCESSNODE = "";
int    MAX_THREADS   = 1;
int    BATCHSIZE     = 1024;
int    BATCHSIZETA   = 8;
int    MTSAFEDIST    = 2000;
int    DRCSAFEDIST   = 500;
int    VERBOSE       = 1;
int    BOTTOM_ROUTING_LAYER = 2;
//表示路由过程中可使用的最高层级
//使用了标准库中的std::numeric_limits模板类，它提供了关于数值类型极限值的信息
//极限值：std::numeric_limits<frLayerNum>::max()返回frLayerNum类型所能达到的最大值。由于frLayerNum是int的别名，这将返回int类型的最大值
//在io_parser_helper.cpp的io::Parser::postProcess()函数中被设置为18
int    TOP_ROUTING_LAYER = std::numeric_limits<frLayerNum>::max();
bool   ALLOW_PIN_AS_FEEDTHROUGH = false;
bool   USENONPREFTRACKS = true;
bool   USEMINSPACING_OBS = true;
bool   RESERVE_VIA_ACCESS = true;
bool   ENABLE_BOUNDARY_MAR_FIX = true;
bool   ENABLE_VIA_GEN = true;

//同TOP_ROUTING_LAYER，下边两个变量被设置为2
//用于指定与“via in pin”（在引脚中的via）相关的最低层级 = 2
frLayerNum VIAINPIN_BOTTOMLAYERNUM             = std::numeric_limits<frLayerNum>::max();
//用于指定与“via in pin”（在引脚中的via）相关的最高层级 = 2
frLayerNum VIAINPIN_TOPLAYERNUM                = std::numeric_limits<frLayerNum>::max();
// frLayerNum VIAINPIN_BOTTOMLAYERNUM             = 2;
// frLayerNum VIAINPIN_TOPLAYERNUM                = 2;

//很可能用于指定与宏单元引脚（macro cell pin）相关的最低层级 = 2
frLayerNum VIAONLY_MACROCELLPIN_BOTTOMLAYERNUM = std::numeric_limits<frLayerNum>::max();
//很可能用于指定与宏单元引脚（macro cell pin）相关的最高层级 = 2
frLayerNum VIAONLY_MACROCELLPIN_TOPLAYERNUM    = std::numeric_limits<frLayerNum>::max();
//frLayerNum VIAONLY_STDCELLPIN_BOTTOMLAYERNUM   = std::numeric_limits<frLayerNum>::max();
//frLayerNum VIAONLY_STDCELLPIN_TOPLAYERNUM      = std::numeric_limits<frLayerNum>::max();
// frLayerNum VIAONLY_STDCELLPIN_BOTTOMLAYERNUM   = 2;
// frLayerNum VIAONLY_STDCELLPIN_TOPLAYERNUM      = 2;

//可能表示每个宏单元引脚（macro cell pin）的最小访问点（access point）数量 = 3
int MINNUMACCESSPOINT_MACROCELLPIN      = 3;
//可能表示每个标准单元引脚（standard cell pin）的最小访问点数量 = 3
int MINNUMACCESSPOINT_STDCELLPIN        = 3;
// int        MINNUMACCESSPOINT_STDCELLPIN        = 5;
//可能表示访问模式（access pattern）结束时的迭代次数 = 5
int ACCESS_PATTERN_END_ITERATION_NUM    = 5;

//下边的几个变量是用户输入参数
frLayerNum ONGRIDONLY_WIRE_PREF_BOTTOMLAYERNUM    = std::numeric_limits<frLayerNum>::max();
frLayerNum ONGRIDONLY_WIRE_PREF_TOPLAYERNUM       = std::numeric_limits<frLayerNum>::min();
frLayerNum ONGRIDONLY_WIRE_NONPREF_BOTTOMLAYERNUM = std::numeric_limits<frLayerNum>::max();
frLayerNum ONGRIDONLY_WIRE_NONPREF_TOPLAYERNUM    = std::numeric_limits<frLayerNum>::min();
frLayerNum ONGRIDONLY_VIA_BOTTOMLAYERNUM          = std::numeric_limits<frLayerNum>::max();
frLayerNum ONGRIDONLY_VIA_TOPLAYERNUM             = std::numeric_limits<frLayerNum>::min();

//可能是用于访问点（access points）的层级
frLayerNum VIA_ACCESS_LAYERNUM = 2;

//最大迭代次数
int END_ITERATION = 80;

frUInt4 TAVIACOST       = 1;  //轨道分配通孔成本 = 1
frUInt4 TAPINCOST       = 4;  //轨道分配引脚成本 = 4
frUInt4 TAALIGNCOST     = 4;  //轨道分配对齐成本 = 4
frUInt4 TADRCCOST       = 32; //轨道分配DRC成本 = 32
//轨道分配形状宽度 = 1.5 - 可能与电路设计中的“形状膨胀”（shape bloat width）有关
//形状膨胀是指在设计对象（如导线、形状或块）周围增加额外的空间，以满足制造过程中的某些要求，例如确保足够的间距或对齐
float   TASHAPEBLOATWIDTH = 1.5;

frUInt4 VIACOST         = 4;  //通孔成本 = 4
// new cost used
frUInt4 GRIDCOST        = 2;  //可能表示与网格对齐相关的成本或惩罚值 = 2
frUInt4 SHAPECOST       = 8;  //可能与形状（如导线或块）相关的成本 = 8
frUInt4 DRCCOST         = 8;  //可能表示DRC的基本成本 = 8
frUInt4 MARKERCOST      = 32; //可能表示与放置标记（marker）相关的成本，标记用于指示特定的设计约束或要求 = 32
frUInt4 MARKERBLOATWIDTH= 1;  //可能表示与标记相关的膨胀宽度，用于在设计中为标记留出额外空间 = 1
frUInt4 BLOCKCOST       = 32; //可能表示与块（block）相关的成本 = 32
// disabled change getNextPathCost to enable
//可能表示与布线引导（guide）相关的成本，引导用于指导布线的方向和路径 = 1
frUInt4 GUIDECOST       = 1;  
float   MARKERDECAY     = 0.8;//可能表示标记成本的衰减率或影响范围随距离增加而减少的比例 = 0.8
float   SHAPEBLOATWIDTH = 3;//可能表示形状膨胀的宽度，用于在设计中为形状留出额外空间 = 3
int     MISALIGNMENTCOST = 8;//可能表示与对齐误差相关的成本，用于惩罚布线或形状的不对齐情况 = 8

int CONGCOST = 8; //GR 可能代表设计中拥堵（Congestion）的成本 = 8
int HISTCOST = 32;//GR 可能代表与历史（History）相关的成本 = 32

//重载<<运算符，用于输出frPoint类型的对象，具有x()和y()方法来获取点的横纵坐标
ostream& operator<< (ostream& os, const frPoint &pIn) {
  os <<"( " <<pIn.x() <<" " <<pIn.y() <<" )";
  return os;
}

//重载<<运算符，用于输出frRect类型的矩形对象
ostream& operator<< (ostream& os, const frRect &pinFigIn) {
  //首先检查 pinFigIn 是否关联了一个 frPin 对象（可能表示一个引脚）。如果存在关联的引脚，它将输出引脚的名称和层号
  if (pinFigIn.getPin()) {  //获得一个指针，如果该指针为空，则不输出
    os <<"PINFIG (PINNAME/LAYER) " <<pinFigIn.getPin()->getTerm()->getName() <<" " <<pinFigIn.getLayerNum() <<endl;
  }
  frBox tmpBox;//声明一个 frBox 类型的变量 tmpBox，它可能表示一个矩形框或边界框
  pinFigIn.getBBox(tmpBox); //调用 getBBox() 方法获取 pinFigIn 的边界框，并将其存储在 tmpBox 中
  //输出矩形框的详细信息，包括矩形框的左、下、右、上边界坐标
  os <<"  RECT " <<tmpBox.left() <<" " <<tmpBox.bottom() <<" " <<tmpBox.right() <<" " <<tmpBox.top();
  return os;
}

//重载<<运算符，用于输出frPolygon类型的对象 - 用于表示二维空间中的多边形
ostream& operator<< (ostream& os, const frPolygon &pinFigIn) {
  if (pinFigIn.getPin()) {
    os <<"PINFIG (NAME/LAYER) " <<pinFigIn.getPin()->getTerm()->getName() <<" " <<pinFigIn.getLayerNum() <<endl;
  }
  os <<"  POLYGON";//输出 "POLYGON" 标签，表示接下来输出的是多边形的顶点信息
  for (auto &m: pinFigIn.getPoints()) {//通过 getPoints() 方法遍历 pinFigIn 的所有顶点
    os <<" ( " <<m.x() <<" " <<m.y() <<" )";//对于每个顶点 m，输出其 x 和 y 坐标
  }
  return os;
}

//重载<<运算符，用于输出frPin类型的对象
ostream& operator<< (ostream& os, const frPin &pinIn) {
  os <<"PIN (NAME) " <<pinIn.getTerm()->getName();  //输出引脚的名称
  for (auto &m: pinIn.getFigs()) {  //遍历引脚的所有 pinFig 对象
    if (m->typeId() == frcRect) {   //如果 pinFig 对象是一个矩形
      os <<endl <<*(static_cast<frRect*>(m.get())); //将对象转换为 frRect 类型的指针，并输出
    } else if (m->typeId() == frcPolygon) { //对象转换为 frRect 类型的指针，并输出
      os <<endl <<*(static_cast<frPolygon*>(m.get()));
    } else {  //类型不支持，则输出错误消息 "Unsupported pinFig object!"
      os <<endl <<"Unsupported pinFig object!";
    }
  }
  return os;
}

//重载<<运算符，用于输出frTerm类型的对象
ostream& operator<< (ostream& os, const frTerm &termIn) {
  frString name;    //Term名
  frString netName; //线网名
  name = termIn.getName();   // A, B, Z, VSS, VDD
  if (termIn.getNet()) {    // 在之后设置，instTerm中的term没有net
    netName = termIn.getNet()->getName();
  }
  os <<"TERM (NAME/NET) " <<name <<" " <<netName;
  for (auto &m: termIn.getPins()){  //遍历term的所有引脚并输出
    os <<endl <<*m;
  }
  return os;
}

//重载<<运算符，用于输出frInstTerm类型的对象
ostream& operator<< (ostream& os, const frInstTerm &instTermIn) {
  frString name;    //InstTerm名
  frString cellName;//Cell名
  frString termName;//Term名
  frString netName; //线网名
  name = instTermIn.getInst()->getName();
  cellName = instTermIn.getInst()->getRefBlock()->getName();
  termName = instTermIn.getTerm()->getName();
  if (instTermIn.getNet()) {
    netName = instTermIn.getNet()->getName();
  } 
  os <<"INSTTERM (NAME/CELL/TERM/NET) " <<name <<" " <<cellName <<" " <<termName <<" " <<netName <<endl;
  os <<*instTermIn.getTerm();
  return os;
}

//重载<<运算符，用于输出ViaDef类型的对象 - 通孔一般包含上层、下层和cut层，所以输出三层
ostream& operator<< (ostream& os, const frViaDef &viaDefIn) {
  frString name;  //ViaDef名
  name = viaDefIn.getName();
  os <<"VIA " <<name;
  if (viaDefIn.getDefault()) {  //默认false
    os <<" DEFAULT";
  }
  for (auto &m: viaDefIn.getLayer1Figs()) { //Layer1层的图形
    if (m->typeId() == frcRect) { //矩形
      os <<endl <<*(static_cast<frRect*>(m.get()));
    } else if (m->typeId() == frcPolygon) { //多边形
      os <<endl <<*(static_cast<frPolygon*>(m.get()));
    } else {  //不支持的类型
      os <<endl <<"Unsupported pinFig object!";
    }
  }
  for (auto &m: viaDefIn.getCutFigs()) {  //cut层的图像
    if (m->typeId() == frcRect) { //矩形
      os <<endl <<*(static_cast<frRect*>(m.get()));
    } else if (m->typeId() == frcPolygon) { //多边形
      os <<endl <<*(static_cast<frPolygon*>(m.get()));
    } else {  //不支持的类型
      os <<endl <<"Unsupported pinFig object!";
    }
  }
  for (auto &m: viaDefIn.getLayer2Figs()) { //Layer2层的图形
    if (m->typeId() == frcRect) { //矩形
      os <<endl <<*(static_cast<frRect*>(m.get()));
    } else if (m->typeId() == frcPolygon) { //多边形
      os <<endl <<*(static_cast<frPolygon*>(m.get()));
    } else {  //不支持的类型
      os <<endl <<"Unsupported pinFig object!";
    }
  }
  return os;
}

//ostream& operator<< (ostream& os, const frLayerBlockage &blkIn) {
//  os <<"BLK (LAYER) " <<blkIn.getLayerNum();
//  for (auto &point: blkIn.getPoints()) {
//    os <<endl <<"  " <<point.x() <<" " <<point.y();
//  }
//  return os;
//}

//重载<<运算符，用于输出frBlock类型的对象
ostream& operator<< (ostream& os, const frBlock &blockIn) {
  frBox box;
  blockIn.getBBox(box); //获取块边界box
  os <<"MACRO "    <<blockIn.getName() <<endl
       <<"  ORIGIN " <<box.left()  <<" " <<box.bottom() <<endl
       <<"  SIZE "   <<box.right() <<" " <<box.top();
  for (auto &m: blockIn.getTerms()) {
    os <<endl <<*m;
  }
  //for (auto &m: blockIn.getBlockages()) {
  //  if (m->typeId() == frcLayerBlockage) {
  //    os <<endl <<*(static_cast<frLayerBlockage*>(m.get()));
  //  } else {
  //    os <<endl <<"Unsupported macro blockage!";
  //  }
  //}
  return os;
}

//重载<<运算符，用于输出frInst类型的对象
ostream& operator<< (ostream& os, const frInst &instIn) {
  frPoint tmpPoint;   //临时点
  frString tmpString; //临时字符串
  frString tmpName;   //临时名字
  instIn.getOrigin(tmpPoint);
  auto tmpOrient = instIn.getOrient();
  tmpName = instIn.getName();
  tmpString = instIn.getRefBlock()->getName();
  os <<"- " <<tmpName <<" " <<tmpString <<" + STATUS + ( "
       <<tmpPoint.x() <<" " <<tmpPoint.y() <<" ) " <<tmpOrient.getName() <<endl;
  for (auto &m: instIn.getInstTerms()) {
    os <<endl <<*m;
  }
  return os;
}

//重载<<运算符，用于输出frBox类型的对象
ostream& operator<< (ostream& os, const frBox &box) {
  os <<"( " <<box.left() <<" " <<box.bottom() <<" ) ( " <<box.right() <<" " <<box.top() <<" )";
  return os;
}

