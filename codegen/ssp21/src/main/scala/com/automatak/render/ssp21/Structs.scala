/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages._
import com.automatak.render.ssp21.messages.generators.StructGenerator


object Structs {

  def files: List[WriteCppFiles] = List(SessionAuthData).map(x => StructGenerator(x))

}
