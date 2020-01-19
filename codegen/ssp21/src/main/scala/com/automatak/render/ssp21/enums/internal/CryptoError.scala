/**
 * License TBD
 */
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
    EnumValue("ok", 0, "no error"),
    EnumValue("bad_key_type", 1, "attempted operation with bad key type"),
    EnumValue("bad_length", 2, "attempted operation with a buffer with the wrong length"),
    EnumValue("dh_x25519_fail", 3, "X25519 DH operation failed. Bad public key?"),
    EnumValue("undefined_algorithm", 4, "a procedure cannot be executed b/c of an undefined algorithm"),
    EnumValue("mac_auth_fail", 5, "A calculated MAC did not match the specified MAC"),
    EnumValue("signature_auth_fail", 6, "A calculated digital signature did not authenticate"),
    EnumValue("bad_buffer_size", 7, "A buffer had an insufficient or incorrect size"),
    EnumValue("no_valid_session", 8, "No validate session for authenticating user data"),
    EnumValue("expired_ttl", 9, "The TTL on a message is expired"),
    EnumValue("nonce_replay", 10, "The nonce on a received message has been replayed"),
    EnumValue("max_nonce_exceeded", 11, "A nonce has reached a configured maximum value"),
    EnumValue("max_session_time_exceeded", 12, "The session time has reached a configured maximum value"),
    EnumValue("empty_user_data", 13, "Received a session message with no user data"),
    EnumValue("clock_rollback", 14, "A rollback of the monotonic clock was detected"),
    EnumValue("aead_encrypt_fail", 15, "AEAD encryption failed in the underlying implementation"),
    EnumValue("aead_decrypt_fail", 16, "AEAD authentication failed in the underlying implementation"),
  )

}



