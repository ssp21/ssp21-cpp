/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumFromType extends HeaderImplModelRender[EnumModel] {

  def header: ModelRenderer[EnumModel] = HeaderRender
  def impl: ModelRenderer[EnumModel]  = ImplRender

  private object HeaderRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {
      "static %s from_type(%s arg);".format(em.name, em.cpp.typ).iter
    }
  }

  private object ImplRender extends ModelRenderer[EnumModel] {

    def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {

      def sig = "%s %s::from_type(%s arg)".format(em.name, em.specName, em.cpp.typ).iter
      def smr = new SwitchModelRenderer[EnumValue](ev => em.render(ev.value))(ev => em.qualified(ev))
      def switch = smr.render(em.nonDefaultValues, em.default)

      sig ++ bracket {
        switch
      }
    }
  }
}


