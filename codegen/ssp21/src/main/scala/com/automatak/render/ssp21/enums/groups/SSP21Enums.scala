/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.groups

import com.automatak.render.cpp.EnumConfig
import com.automatak.render.ssp21.enums._


object SSP21Enums {

  def apply() : List[EnumConfig] = fullEnums

  private def fullEnums = List(
    Function()
  ).map(x => EnumConfig(x, true, true))

}
