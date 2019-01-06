/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.cpp

import com.automatak.render._

object EnumFromType extends HeaderImplModelRender[EnumModel] {

  def header: Renderer[EnumModel] = HeaderRender

  def impl: Renderer[EnumModel] = ImplRender

  private object HeaderRender extends Renderer[EnumModel] {
    def render(em: EnumModel)(implicit i: Indentation): Iterator[String] = {
      "static %s from_type(%s arg);".format(em.name, em.enumType.typ).iter
    }
  }

  private object ImplRender extends Renderer[EnumModel] {

    def render(em: EnumModel)(implicit i: Indentation): Iterator[String] = {

      def sig = "%s %s::from_type(%s arg)".format(em.name, em.specName, em.enumType.typ).iter
      def smr = new SwitchModelRenderer[EnumValue](ev => em.render(ev.value))(ev => em.qualified(ev))
      def switch = smr.render(em.nonDefaultValues, em.default)

      sig ++ bracket {
        switch
      }
    }
  }

}


