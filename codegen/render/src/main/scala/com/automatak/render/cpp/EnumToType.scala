/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumToType extends HeaderImplModelRender[EnumModel] {

  def header: ModelRenderer[EnumModel] = HeaderRender
  def impl: ModelRenderer[EnumModel] =  ImplRender

  private def signature(em: EnumModel) : String = List(getEnumType(em.enumType), List(em.underscoredName,"_to_type(", em.name," arg)").mkString).mkString(" ")

  private object HeaderRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = Iterator(signature(em)+";")
  }

  private object ImplRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = {
      Iterator(signature(em)) ++ bracket {
        Iterator(List("return ", staticCast(em.enumType)("arg"),";").mkString)
      }
    }
  }

}


