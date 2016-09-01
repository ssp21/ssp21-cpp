/**
 * License TBD
 */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages._
import com.automatak.render.ssp21.messages.generators.BitfieldStructGenerator


object Bitfields {

  val payloadFlags = Bitfield(
    "payload_flags", "PayloadFlags",
    List(
      Bit("fir", true),
      Bit("fin", true)
    )
  )

  def files : List[WriteCppFiles] = List(
    payloadFlags
  ).map(BitfieldStructGenerator.apply)

}
