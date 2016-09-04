/**
  * License TBD
  */
package com.automatak.render.cpp

import com.automatak.render.EnumModel

case class EnumConfig(
                       model: EnumModel,
                       conversions: Boolean,
                       stringConv: Boolean,
                       isErrorEnum: Boolean = false) {
  def anyOptionalFunctions: Boolean = conversions || stringConv
}

