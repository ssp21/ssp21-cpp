/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object CryptoError {

  private val comments = List("Various errors in the cryptographic layer")

  def category = Some(ErrorCategory("CryptoErrorCategory", "crypto error"))

  def apply(): EnumModel = EnumModel("CryptoError", "crypto_error", comments, EnumType.UInt8, codes, None, None, Hex, category)

  private val codes = List(
    EnumValue("bad_key_type", 0, "attempted operation with bad key type"),
    EnumValue("dh_x25519_fail", 1, "X25519 DH operation failed. Bad public key?"),
    EnumValue("undefined_algorithm", 2, "a procedure cannot be executed b/c of an undefined algorithm"),
    EnumValue("mac_auth_fail", 3, "A calculated MAC did not match the specified MAC"),
    EnumValue("bad_buffer_size", 4, "A buffer had an insufficient or incorrect size"),
    EnumValue("no_valid_session", 5, "No validate session for authenticating user data"),
    EnumValue("expired_ttl", 6, "The TTL on a message is expired"),
    EnumValue("nonce_replay", 7, "The nonce on a received message has been replayed"),
    EnumValue("nonce_config_max", 8, "A nonce has reached a configured maximum value or the intended operation would exceed the configured maximum"),
    EnumValue("ttl_overflow", 9, "The TTL could not be set because the value would overflow the representation"),
    EnumValue("empty_user_data", 10, "Received a session message with no user data")
  )

}



