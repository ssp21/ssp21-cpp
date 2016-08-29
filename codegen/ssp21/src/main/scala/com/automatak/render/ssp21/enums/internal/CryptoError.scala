/**
 * License TBD
 */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object CryptoError {

  private val comments = List("The result of a message parse operation")

  def apply(): EnumModel = EnumModel("CryptoError", "crypto_error", comments, EnumModel.UInt8, codes, None)

  private val codes = List(
    EnumValue("bad_key_type", 0, "attempted operation with bad key type"),
    EnumValue("dh_x25519_fail", 1, "X25519 DH operation failed. Bad public key?"),
    EnumValue("undefined_algorithm", 2, "a procedure cannot be executed b/c of an undefined algorithm")
  )

}



