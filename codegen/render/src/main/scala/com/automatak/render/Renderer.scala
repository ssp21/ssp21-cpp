/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render

/**
  * Converts some type into a line iterator
  *
  * @tparam A
  */
trait Renderer[A] {

  def render(a: A)(implicit i: Indentation): Iterator[String]

}
