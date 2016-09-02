/**
  * License TBD
  */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages._


object Bitfields {

  val sessionFlags: Bitfield = Bitfield(
    "flags", "SessionFlags",
    Bit("fir", true),
    Bit("fin", true)
  )

  def files: List[WriteCppFiles] = List(
    sessionFlags
  ).map(_.files)

}
