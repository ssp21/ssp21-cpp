/**
 * License TBD
 */
/**
  * License TBD
  */
package com.automatak.render

package object cpp {

  def namespace(ns: String)(internals: Iterator[String]): Iterator[String] = {

    "namespace %s {".format(ns).iter ++
      space ++
      internals ++
      space ++
      Iterator.apply("}")

  }

  def include(s: String): Iterator[String] = "#include %s".format(s).iter

  def includeGuards(fileName: String)(inner: => Iterator[String]): Iterator[String] = {
    includeGuardsRaw("SSP21_" + fileName.toUpperCase + "_H")(inner)
  }

  def includeGuardsRaw(name: String)(inner: => Iterator[String]): Iterator[String] = {

    "#ifndef %s".format(name).iter ++
      "#define %s".format(name).iter ++
      space ++
      inner ++
      space ++
      "#endif".iter

  }

}
