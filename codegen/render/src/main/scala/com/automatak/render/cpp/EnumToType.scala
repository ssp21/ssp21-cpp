/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render._

object EnumToType extends HeaderImplModelRender[EnumModel] {

  def header: ModelRenderer[EnumModel] = HeaderRender
  def impl: ModelRenderer[EnumModel] =  ImplRender

  private object HeaderRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = {
      "static %s to_type(%s arg);".format(em.enumType.cppType, em.name).iter
    }
  }

  private object ImplRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit indent: Indentation) : Iterator[String] = {
      "%s %s::to_type(%s arg)".format(em.enumType.cppType, em.specName, em.name).iter ++ bracket {
        "return static_cast<%s>(arg);".format(em.enumType.cppType).iter
      }
    }
  }

}


