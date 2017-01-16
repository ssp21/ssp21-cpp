/**
  * License TBD
  */
package com.automatak.render.cpp

import com.automatak.render._

object EnumToType extends HeaderImplModelRender[EnumModel] {

  def header: Renderer[EnumModel] = HeaderRender

  def impl: Renderer[EnumModel] = ImplRender

  private object HeaderRender extends Renderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation): Iterator[String] = {
      "static %s to_type(%s arg);".format(em.enumType.typ, em.name).iter
    }
  }

  private object ImplRender extends Renderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation): Iterator[String] = {

      def sig = "%s %s::to_type(%s arg)".format(em.enumType.typ, em.specName, em.name).iter
      def inner = "return static_cast<%s>(arg);".format(em.enumType.typ).iter

      sig ++ bracket {
        inner
      }
    }
  }

}


