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

//包含了一些实用工具函数的实现，这些函数主要用于打印和展示电路设计中的各种组件和信息

//#include "FlexRoute.h"
#include <iostream>
#include <fstream>
#include "global.h"
#include "frDesign.h"
using namespace std;
using namespace fr;

//实现frDesign类的成员函数，打印所有组件
void frDesign::printAllComps() {
  cout <<endl <<"print all components: ";
  // 遍历 topBlock 中的所有实例（insts）
  for (auto &m: topBlock->getInsts()) {
    cout <<endl <<*m <<endl;  //此处能输出是因为在global.cpp中实现了重载
  }
}

// 实现 frDesign 类的成员函数，打印所有宏单元
void frDesign::printAllMacros() {
  cout <<endl;
  cout <<"print all macros: " <<endl;
  // 遍历 refBlocks 中的所有引用块
  for (auto &m: refBlocks) {
    cout <<endl <<*(m) <<endl;
  }
}

// 被注释掉的函数，可能是用于打印所有网络的函数
////void FlexRoute::printAllNets() {
////  cout <<endl;
////  cout <<"print all nets: " <<endl;
////  for (auto &m: nets) {
////    cout <<endl <<*(m.second) <<endl;
////  }
////}

// 实现 frDesign 类的成员函数，打印所有终端
void frDesign::printAllTerms() {
  cout <<endl <<"print all terminals: ";
  // 遍历 topBlock 中的所有终端（terms）
  for (auto &m: topBlock->getTerms()) {
    cout <<endl <<*m <<endl;
  }
}

// 实现 frTechObject 类的成员函数，打印所有通孔（vias）
void frTechObject::printAllVias() {
  cout <<endl <<"print all vias: " <<endl;
  // 遍历 vias 容器中的所有通孔
  for (auto &m: vias) {
    cout <<endl <<*(m) <<endl;
  }
}

// 定义成员函数 printCMap，没有返回值，也没有参数
void frDesign::printCMap() {
  // 打印标题
  cout << endl << "print cmap: ";
  // 初始化计数器 xCnt 和 yCnt，用于存储水平和垂直 GCell 的数量
  int xCnt = 0;
  int yCnt = 0;
  
  // 遍历 topBlock 中的 GCell 模式 - gcp从vector中获取元素
  for (auto &gcp : topBlock->getGCellPatterns()) {
    // 如果 GCell 模式是水平的，则更新 y 计数器
    if (gcp.isHorizontal()) {
      yCnt = gcp.getCount();
    } 
    // 否则（GCell 模式是垂直的），更新 x 计数器
    else {
      xCnt = gcp.getCount();
    }
  }
  
  // 打印 GCell 的数量信息
  cout << "x/y = " << xCnt << " " << yCnt << endl;
  
  // 获取 topBlock 的拥塞图（cmap）
  auto &cmap = topBlock->getCMap();
  
  // 遍历所有层（假设层数是偶数，从 0 开始，每次增加 2）
  for (int lNum = 0; lNum <= (int)getTech()->getLayers().size(); lNum += 2) {
    // 对于每个 GCell，打印其拥塞图(cmap)的详细信息
    for (int i = 0; i < xCnt; i++) {
      for (int j = 0; j < yCnt; j++) {
        // 打印 GCell 的坐标和层号
        cout << "x/y/z/s/t/l/e1/e2/u = " 
             << i << " " << j << " " << lNum << " ";
        
        // 打印每个 GCell 的拥塞图(cmap)数据
        // getSupply, getThroughDemand, getLocalDemand, getEdge1Demand, 
        // getEdge2Demand, getUpDemand 是 cmap 对象提供的方法，用于获取不同类型的电容值
        cout << cmap.getSupply(i, j, lNum) << " " //供给
             << cmap.getThroughDemand(i, j, lNum) << " "  //Through需求
             << cmap.getLocalDemand(i, j, lNum) << " "  //Local需求
             << cmap.getEdge1Demand(i, j, lNum) << " "  //边1需求
             << cmap.getEdge2Demand(i, j, lNum) << " "  //边2需求
             << cmap.getUpDemand(i, j, lNum) << " ";    //Up需求
        
        // 如果发现 GCell 的总需求超过供给，则打印 "GCell overflow"表示溢出
        if (cmap.getThroughDemand(i, j, lNum) + cmap.getLocalDemand(i, j, lNum) +
            cmap.getEdge1Demand(i, j, lNum) > cmap.getSupply(i, j, lNum) ||
            cmap.getThroughDemand(i, j, lNum) + cmap.getLocalDemand(i, j, lNum) +
            cmap.getEdge2Demand(i, j, lNum) > cmap.getSupply(i, j, lNum)) {
          cout << "GCell overflow";
        }
        // 换行，准备打印下一个 GCell 的信息
        cout << endl;
      }
    }
  }
}