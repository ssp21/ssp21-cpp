/**
 * License TBD
 */
package com.automatak.render.cpp

import com.automatak.render.EnumModel

case class EnumConfig(model: EnumModel, conversions: Boolean, stringConv: Boolean) {
  def anyOptionalFunctions : Boolean = conversions || stringConv
}

