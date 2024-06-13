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

//FlexRoute类的实现，这个类可能负责执行路由算法。
//它初始化设计，执行布线前的准备，执行全局路由（GR）、轨道分配（TA）、详细布线（DR）等步骤

#include <iostream>
#include "global.h"
#include "FlexRoute.h"
#include "io/io.h"
#include "pa/FlexPA.h"
#include "ta/FlexTA.h"
#include "dr/FlexDR.h"
//#include "io/frPinPrep.h"
#include "gc/FlexGC.h"
#include "gr/FlexGR.h"
#include "rp/FlexRP.h"

using namespace std;
using namespace fr;

// FlexRoute类的构造函数，初始化设计对象
void FlexRoute::init() {
  io::Parser parser(getDesign());
  parser.readLefDef();  // 读取LEF和DEF文件，用于获取设计的基本数据
  if (GUIDE_FILE != string("")) {// 如果指定了GUIDE_FILE，则读取引导文件
    parser.readGuide();
  } else {// 否则禁止生成via
    ENABLE_VIA_GEN = false;
  }
  parser.postProcess();// 执行一些后置处理
  FlexPA pa(getDesign());// 创建FlexPA对象，用于执行放置算法
  pa.main(); // 调用主函数
  if (GUIDE_FILE != string("")) {// 如果有引导文件，进行后置处理
    parser.postProcessGuide();
  }
  // GR-related
  parser.initRPin(); // 初始化RPin
}

// FlexRoute类的成员函数，用于准备路由前的准备工作
void FlexRoute::prep() {
  FlexRP rp(getDesign(), getDesign()->getTech());// 创建FlexRP对象，用于执行布线前的准备
  rp.main();// 调用主函数
}

// FlexRoute类的成员函数，用于执行全局路由（GR）
void FlexRoute::gr() {
  FlexGR gr(getDesign()); // 创建FlexGR对象，用于全局路由
  gr.main();
}

// FlexRoute类的成员函数，用于执行轨道分配（TA）
void FlexRoute::ta() {
  FlexTA ta(getDesign());// 创建FlexTA对象，用于轨道分配
  ta.main();
  io::Writer writer(getDesign());// 创建Writer对象，用于写入数据
  writer.writeFromTA();// 从轨道分配写入数据
}

// FlexRoute类的成员函数，用于执行详细布线（DR）
void FlexRoute::dr() {
  FlexDR dr(getDesign());// 创建FlexDR对象，用于详细布线
  dr.main();
}

// FlexRoute类的成员函数，用于完成布线后的收尾工作
void FlexRoute::endFR() {
  io::Writer writer(getDesign());// 创建Writer对象，用于写入数据
  writer.writeFromDR();// 从详细布线写入数据
  if (REF_OUT_FILE != DEF_FILE) {// 如果引用输出文件与定义文件不同，则删除引用输出文件
    remove(REF_OUT_FILE.c_str());
  }
}

// FlexRoute类的main函数，是程序执行的主要入口点
int FlexRoute::main() {
  init();// 初始化
  if (GUIDE_FILE == string("")) {// 如果没有指定GUIDE_FILE
    gr();// 执行全局路由
    io::Parser parser(getDesign());// 创建Parser对象
    GUIDE_FILE = OUTGUIDE_FILE;// 设置GUIDE_FILE为输出引导文件
    ENABLE_VIA_GEN = true;// 允许生成via
    parser.readGuide(); // 读取引导文件
    parser.initDefaultVias();// 初始化默认via
    parser.writeRefDef();// 写入参考定义
    parser.postProcessGuide();// 后置处理引导文件
  }
  prep();// 准备路由
  ta(); //轨道分配
  dr(); //详细布线
  endFR();  //收尾工作


  /*
  // rtree test
  vector<rtree_frConnFig_value_t> result1;
  design->getTopBlock()->queryRtree4Routes(frBox(585000, 1098000, 590000, 1101000), 6, result1);
  cout <<endl <<"query1:" <<endl;
  for (auto &it: result1) {
    if (it.second->typeId() == frcPathSeg) {
      frPoint pt1, pt2;
      dynamic_pointer_cast<frPathSeg>(it.second)->getPoints(pt1, pt2);
      cout <<"found pathseg " <<pt1.x() <<" " <<pt1.y() <<" " << pt2.x() <<" " <<pt2.y() 
           <<" " <<dynamic_pointer_cast<frPathSeg>(it.second)->getNet()->getName() <<endl;
    } else if (it.second->typeId() == frcGuide) {
      frPoint pt1, pt2;
      dynamic_pointer_cast<frGuide>(it.second)->getPoints(pt1, pt2);
      cout <<"found guide   " <<pt1.x() <<" " <<pt1.y() <<" " << pt2.x() <<" " <<pt2.y()
           <<" " <<dynamic_pointer_cast<frGuide>(it.second)->getNet()->getName();
      if (dynamic_pointer_cast<frGuide>(it.second)->getBeginLayerNum() !=
          dynamic_pointer_cast<frGuide>(it.second)->getEndLayerNum()) {
        cout <<" via guide";
      }
      cout <<endl;
    }
  }
  exit(0);
  */
  return 0;
}

