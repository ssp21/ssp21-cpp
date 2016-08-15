package com.automatak.render.ssp21.messages

import com.automatak.render.ssp21.enums._


object ReplyHandshakeBegin extends Message {

  override def name : String = "ReplyHandshakeBegin"

  override def fields: List[Field] = List(
      Enum(CryptoFunction()),
      U16("version"),
      Seq8ofU8("ephemeral_public_key"),
      Seq8ofSeq16ofU8("certificates")
  )

}
