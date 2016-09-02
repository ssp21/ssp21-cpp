/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages.SessionFlags
import com.automatak.render.ssp21.messages.generators.BitfieldStructGenerator


object Bitfields {

  def files: List[WriteCppFiles] = List(
    SessionFlags
  ).map(x => BitfieldStructGenerator(x))

}
