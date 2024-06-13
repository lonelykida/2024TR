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

//声明了frDesign类，
//它代表整个设计，并包含对设计中的不同组件（如顶层块、技术对象、区域查询对象等）的引用

#ifndef _FR_DESIGN_H_
#define _FR_DESIGN_H_

#include <memory>
#include "global.h"
#include "frBaseTypes.h"
#include "db/obj/frBlock.h"
#include "db/tech/frTechObject.h"
#include "frRegionQuery.h"

namespace fr {  //包含命名空间io(io中有类Parser)以及嘞frDesign
  namespace io {
    class Parser;
  }
  class frDesign {
  public:
    // constructors - 无参构造
    frDesign(): topBlock(nullptr), tech(std::make_unique<frTechObject>()), 
                rq(std::make_unique<frRegionQuery>(this)) {}
    // getters - 获取器
    frBlock* getTopBlock() const {  //获取顶层块
      return topBlock.get();  //获取智能指针当前拥有的对象的原始指针，不改变所有权
    }
    frTechObject* getTech() const { //获取Tech
      return tech.get();
    }
    frRegionQuery* getRegionQuery() const { //获取区域查询对象
      return rq.get();
    }
    //提供const与非const版本的接口
    std::vector<std::unique_ptr<frBlock> >& getRefBlocks() {  //获取参考块集
      return refBlocks;
    }
    const std::vector<std::unique_ptr<frBlock> >& getRefBlocks() const {  //获取参考块const集
      return refBlocks;
    }
    // setters - 设置器

    void setTopBlock(std::unique_ptr<frBlock> &in) {
      topBlock = std::move(in); //将in的所有权移交给topBlock
    }
    void addRefBlock(std::unique_ptr<frBlock> &in) {
      //将 in 指向的 frBlock 对象的名称作为键，将 in 原始指针作为值，存入 name2refBlock 映射表中
      name2refBlock[in->getName()] = in.get();  
      //将 in 的所有权转移给 refBlocks 向量。这会导致 in 不再拥有其指向的对象，对象被添加到 refBlocks 向量中
      refBlocks.push_back(std::move(in)); 
    }

    // others
    //打印所有宏块
    void printAllMacros();  
    //打印所有comps
    void printAllComps(); 
    //打印所有terms  
    void printAllTerms();   
    //打印CMap
    void printCMap();     
    //友元Parser  
    friend class io::Parser;
  protected:
  //unique_ptr代表了一个拥有唯一所有权的指针，意味着在任何给定时间，要么没有对象拥有这个资源，要么只有一个对象拥有它
  //当 unique_ptr 被销毁时，它所拥有的资源也会自动释放
    std::unique_ptr<frBlock>                      topBlock; //顶层块
    std::map<frString, frBlock*>                  name2refBlock;//名字到引用块的映射
    std::vector<std::unique_ptr<frBlock> >        refBlocks;//引用块
    std::unique_ptr<frTechObject>                 tech; //技术对象
    std::unique_ptr<frRegionQuery>                rq; //区域查询
  };
}

#endif
