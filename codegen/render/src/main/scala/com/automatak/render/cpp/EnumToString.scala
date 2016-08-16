/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumToString extends HeaderImplModelRender[EnumModel] {

  def impl: ModelRenderer[EnumModel]  = ImplRender
  def header: ModelRenderer[EnumModel]  = HeaderRender

  private def signature(enum: EnumModel) : String = List("static", cString, List("to_string(", enum.name," arg)").mkString).mkString(" ")

  private def implSignature(enum: EnumModel) : String = List(cString, List(enum.specName, "::to_string(", enum.name," arg)").mkString).mkString(" ")

  private object HeaderRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = Iterator(signature(em)+";")
  }

  private object ImplRender extends ModelRenderer[EnumModel] {

    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

      def header = Iterator(implSignature(em))
      def smr = new SwitchModelRenderer[EnumValue](ev => em.qualified(ev))(ev => quoted(ev.displayName))
      def getDefault : EnumValue = em.defaultValue.getOrElse(EnumValue("undefined",0))
      def switch = smr.render(em.nonDefaultValues, getDefault)

      header ++ bracket {
        switch
      }
    }
  }
}


