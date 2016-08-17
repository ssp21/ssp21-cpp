/**
 * License TBD
 */
package com.automatak.render

package object cpp {

  private val quote : String = "\""

  def quoted(s: String): String = List(quote, s, quote).mkString

  def bracketed(s: String): String = List("<", s, ">").mkString

  def namespace(ns: String)(internals: Iterator[String]): Iterator[String] = {

    "namespace %s {".format(ns).iter ++
    space ++
    internals ++
    space ++
    Iterator.apply("}")

  }

  def comment(comment: String)(implicit i: Indentation): Iterator[String] = "// %s".format(comment).iter

  def includeGuards(name: String)(internals: => Iterator[String]): Iterator[String] = {

    val ssp21Pattern  = "SSP21_"+ name.toUpperCase + "_H"

    Iterator("#ifndef %s".format(ssp21Pattern)) ++
    Iterator("#define %s".format(ssp21Pattern)) ++
    space ++
    internals ++
    space ++
    Iterator("#endif")

  }

  def includeGuardsRaw(name: String)(internals: => Iterator[String]): Iterator[String] = {

    Iterator("#ifndef %s".format(name)) ++
      Iterator("#define %s".format(name)) ++
      space ++
      internals ++
      space ++
      Iterator("#endif")

  }

  def include(s: String): Iterator[String] = "#include %s".format(s).iter

}
