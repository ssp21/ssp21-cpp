/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.cpp.EnumConfig
import com.automatak.render.ssp21.enums.generators.EnumGenerator
import com.automatak.render.ssp21.enums.test.CryptoAction


object TestFiles {

  def list : List [WriteCppFiles] = List(
    EnumConfig(CryptoAction(), false, true)
  ).map(c => EnumGenerator(c, Some("")))

}
