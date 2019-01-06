/**
 * License TBD
 */
package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages._

object AuthMetadata extends Struct {

  override def name: String = "AuthMetadata"

  override def fields: List[Field] = List(
    U16("nonce"),
    U32("valid_until_ms")
  )

}
