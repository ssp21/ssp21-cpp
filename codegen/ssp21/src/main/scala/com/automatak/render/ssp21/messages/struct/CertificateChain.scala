/**
 * License TBD
 */
package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages.{Field, SeqOfStruct, Struct}

object CertificateChain extends Struct {

  override def name: String = "CertificateChain"

  override def fields: List[Field] = List(
    SeqOfStruct("certificates", CertificateEnvelope, 6)
  )

}
