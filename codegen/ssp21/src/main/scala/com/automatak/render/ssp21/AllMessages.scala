/**
 * License TBD
 */
package com.automatak.render.ssp21

import com.automatak.render.ssp21.messages._
import com.automatak.render.ssp21.messages.generators.MessageGenerator


object AllMessages {

  def files : List[WriteCppFiles] = List(
    RequestHandshakeBegin,
    ReplyHandshakeBegin,
    RequestHandshakeAuth,
    ReplyHandshakeAuth,
    ReplyHandshakeError,
    UnconfirmedSessionData
  ).map(m => MessageGenerator(m))

}
