/**
  * License TBD
  */
package com.automatak.render.cpp

import com.automatak.render.Renderer

trait HeaderImplModelRender[A] {
  def header: Renderer[A]

  def impl: Renderer[A]
}
