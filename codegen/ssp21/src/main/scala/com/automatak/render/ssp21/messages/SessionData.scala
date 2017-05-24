package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums.ssp21.CryptoFunction
import com.automatak.render.ssp21.messages.struct.AuthMetadata


object SessionData extends Message {

  override def name: String = "SessionData"

  def function = CryptoFunction.sessionData

  override def fields: List[Field] = List(
    StructField("metadata", AuthMetadata),
    SeqOfByte("user_data"),
    SeqOfByte("auth_tag")
  )

}
