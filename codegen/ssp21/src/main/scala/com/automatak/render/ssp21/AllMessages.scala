/**
 * License TBD
 */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages._


object AllMessages {

  def apply() : List[Message] = List(
    RequestHandshakeBegin,
    ReplyHandshakeBegin,
    RequestHandshakeAuth,
    ReplyHandshakeAuth,
    ReplyHandshakeError,
    UnconfirmedSessionData
  )

}
