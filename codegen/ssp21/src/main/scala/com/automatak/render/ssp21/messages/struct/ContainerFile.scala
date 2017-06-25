package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.messages._

object ContainerFile extends Struct {

  override def name: String = "ContainerFile"

  override def fields: List[Field] = List(
    SeqOfStruct("entries", CertificateFileEntry, 10)
  )

}
