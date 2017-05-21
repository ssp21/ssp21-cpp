package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages._

object CertificateFile extends Struct {

  override def name: String = "CertificateFile"

  override def fields: List[Field] = List(
    Seq8OfStruct("entries", CertificateFileEntry, 10)
  )

}
