/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumToString extends HeaderImplModelRender[EnumModel] {

  def impl: Renderer[EnumModel]  = ImplRender
  def header: Renderer[EnumModel]  = HeaderRender

  private object HeaderRender extends Renderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = {
      "static const char* to_string(%s arg);".format(em.name).iter
    }
  }

  private object ImplRender extends Renderer[EnumModel] {

    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

      def sig = "const char* %s::to_string(%s arg)".format(em.specName, em.name).iter
      def smr = new SwitchModelRenderer[EnumValue](ev => em.qualified(ev))(ev => quoted(ev.name))
      def getDefault : EnumValue = em.defaultValue.getOrElse(EnumValue.undefined(0))
      def switch = smr.render(em.nonDefaultValues, getDefault)

      sig ++ bracket {
        switch
      }
    }
  }
}


