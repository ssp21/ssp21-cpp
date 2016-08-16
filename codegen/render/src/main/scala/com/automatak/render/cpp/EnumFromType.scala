/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumFromType extends HeaderImplModelRender[EnumModel] {

  def header: ModelRenderer[EnumModel] = HeaderRender
  def impl: ModelRenderer[EnumModel]  = ImplRender

  private def signature(enum: EnumModel) = List("static", enum.name, List("from_type(", getEnumType(enum.enumType)," arg)").mkString).mkString(" ")

  private def implSignature(enum: EnumModel) = List(enum.name, List(enum.specName,"::from_type(", getEnumType(enum.enumType)," arg)").mkString).mkString(" ")

  private object HeaderRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {
      Iterator(signature(em)+";")
    }
  }

  private object ImplRender extends ModelRenderer[EnumModel] {

    def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {

      def header = Iterator(implSignature(em))
      def smr = new SwitchModelRenderer[EnumValue](ev => em.render(ev.value))(ev => em.qualified(ev))
      def switch = smr.render(em.nonDefaultValues, em.default)

      header ++ bracket {
        switch
      }
    }
  }
}


