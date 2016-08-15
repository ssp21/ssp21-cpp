/**
 * License TBD
 */
package com.automatak.render.ssp21

import com.automatak.render.cpp.EnumConfig
import com.automatak.render.ssp21.enums._


object AllEnums {

  def apply() : List[EnumConfig] = fullEnums

  private def fullEnums = List(
    CryptoFunction(),
    HashMode(),
    NonceMode(),
    DHMode(),
    SessionMode(),
    CertificateMode(),
    HandshakeError()
  ).map(x => EnumConfig(x, true, true))

}
