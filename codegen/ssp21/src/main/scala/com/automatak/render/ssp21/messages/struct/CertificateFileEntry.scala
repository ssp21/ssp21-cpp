package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.enums.ssp21.FileEntryType
import com.automatak.render.ssp21.messages._

object CertificateFileEntry extends Struct {

  override def name: String = "CertificateFileEntry"

  override def fields: List[Field] = List(
    Enum(FileEntryType),
    SeqOfByte("data")
  )

}
