/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.internal

import com.automatak.render._

object CryptoError extends EnumModel {

  override def name: String = "CryptoError"

  override def underscoredName: String = "crypto_error"

  override def comments: List[String] = List("Various errors in the cryptographic layer")

  override def nonDefaultValues: List[EnumValue] = codes

  override def defaultValue: Option[EnumValue] = None

  override def errorCategory: Option[ErrorCategory] = Some(ErrorCategory("CryptoErrorCategory", "crypto error"))

  private val codes = List(
    EnumValue("bad_key_type", 1, "attempted operation with bad key type"),
    EnumValue("dh_x25519_fail", 2, "X25519 DH operation failed. Bad public key?"),
    EnumValue("undefined_algorithm", 3, "a procedure cannot be executed b/c of an undefined algorithm"),
    EnumValue("mac_auth_fail", 4, "A calculated MAC did not match the specified MAC"),
    EnumValue("bad_buffer_size", 5, "A buffer had an insufficient or incorrect size"),
    EnumValue("no_valid_session", 6, "No validate session for authenticating user data"),
    EnumValue("expired_ttl", 7, "The TTL on a message is expired"),
    EnumValue("nonce_replay", 8, "The nonce on a received message has been replayed"),
    EnumValue("max_nonce_exceeded", 9, "A nonce has reached a configured maximum value"),
    EnumValue("max_session_time_exceeded", 10, "The session time has reached a configured maximum value"),
    EnumValue("empty_user_data", 11, "Received a session message with no user data"),
    EnumValue("clock_rollback", 12, "A rollback of the monotonic clock was detected")
  )

}



