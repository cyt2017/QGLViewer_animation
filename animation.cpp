/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.1.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "animation.h"
#include <math.h>
#include <stdlib.h> // RAND_MAX

using namespace qglviewer;
using namespace std;

///////////////////////   V i e w e r  ///////////////////////
void Viewer::init() {
  restoreStateFromFile();//????????????
  glDisable(GL_LIGHTING);//禁用灯光效果
  nbPart_ = 2000;
  particle_ = new Particle[nbPart_];//设置2000个粒子
  glPointSize(3.0);//设置粒子的大小为3.0
  setGridIsDrawn();//绘制格子
  help();//设置帮助信息
  startAnimation();//设置动画的时间
}

void Viewer::draw() {
  glBegin(GL_POINTS);//开始绘制点
  for (int i = 0; i < nbPart_; i++)
    particle_[i].draw();//绘制每一个粒子,粒子的位置和颜色
  glEnd();//结束绘制
}

void Viewer::animate() {
  for (int i = 0; i < nbPart_; i++)
    particle_[i].animate();//让每个点动起来,通过改变粒子的速度和位置
}

QString Viewer::helpString() const {
  QString text("<h2>A n i m a t i o n</h2>");
  text += "Use the <i>animate()</i> function to implement the animation part "
          "of your ";
  text += "application. Once the animation is started, <i>animate()</i> and "
          "<i>draw()</i> ";
  text += "are called in an infinite loop, at a frequency that can be "
          "fixed.<br><br>";
  text += "Press <b>Return</b> to start/stop the animation.";
  return text;
}

///////////////////////   P a r t i c l e   ///////////////////////////////

Particle::Particle() { init(); }

void Particle::animate() {//!animate有生命的; 活的;
  speed_.z -= 0.05f;//speed_(x(0~0.04),y(0~0.04),z(0~1))
  pos_ += 0.1f * speed_;

  if (pos_.z < 0.0) {
    speed_.z = -0.8 * speed_.z;
    pos_.z = 0.0;
  }

  if (++age_ == ageMax_)
    init();
}

void Particle::draw() {
    //绘制顶点颜色，颜色由起始存活位置和最大存活位置
    //该函数中的变量分别为red、green、blue,变量类型是float类型
  glColor3f(age_ / (float)ageMax_, age_ / (float)ageMax_, 1.0);
  //设置顶点位置坐标,该函数的参数是数组
  glVertex3fv(pos_);
}

void Particle::init() {
  pos_ = Vec(0.0, 0.0, 0.0);//Vec是QGLViewer中的变量（三个double类型数据）,设置粒子(Particle)的初始位置
  float angle = 2.0 * M_PI * rand() / RAND_MAX;//rand()获取随机数,RAND_MAX是2的32次方的一半（2147483647）
  float norm = 0.04 * rand() / RAND_MAX;//norm的取值范围是（0～0.04）
  //设置粒子的速度，以norm为半径设置x和y的值,z的取值范围是（0~1）
  speed_ = Vec(norm * cos(angle), norm * sin(angle),
               rand() / static_cast<float>(RAND_MAX));
  age_ = 0;
  //设置粒子的存活距离
  ageMax_ = 50 + static_cast<int>(100.0 * rand() / RAND_MAX);
}
