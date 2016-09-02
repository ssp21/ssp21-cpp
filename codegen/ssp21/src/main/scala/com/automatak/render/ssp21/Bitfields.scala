/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages._
import com.automatak.render.ssp21.messages.generators.BitfieldStructGenerator


object Bitfields {

  object SessionFlags extends Bitfield {

    def name = "SessionFlags"

    def bits = List(
      Bit("fir", true),
      Bit("fin", true)
    )
  }

  def files: List[WriteCppFiles] = List(
    SessionFlags
  ).map(x => BitfieldStructGenerator(x))

}
