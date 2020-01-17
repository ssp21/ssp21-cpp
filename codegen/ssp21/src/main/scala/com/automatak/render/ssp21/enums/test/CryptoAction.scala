/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render.ssp21.enums.test

import com.automatak.render._

object CryptoAction extends EnumModel {

  override def name: String = "CryptoAction"

  override def underscoredName: String = "crypto_actions"

  override def comments: List[String] = List("Enumerates possible cryptographic actions")

  override def nonDefaultValues: List[EnumValue] = EnumValue.list(
    "secure_equals",
    "gen_random",
    "hash_sha256",
    "hmac_sha256",
    "hkdf_sha256",
    "gen_keypair_x25519",
    "dh_x25519",
    "gen_keypair_ed25519",
    "sign_ed25519",
    "verify_ed25519",
    "aes_gcm_encrypt",
    "aes_gcm_decrypt",
  )

  override def defaultValue: Option[EnumValue] = None

}



