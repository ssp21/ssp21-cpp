package com.automatak.render.ssp21.messages.struct

import com.automatak.render.ssp21.enums.ssp21.ContainerEntryType
import com.automatak.render.ssp21.messages._

object ContainerFile extends Struct {

  override def name: String = "ContainerFile"

  override def fields: List[Field] = List(
    Enum(ContainerEntryType),
    SeqOfByte("payload")
  )

}
