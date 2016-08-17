/**
 * License TBD
 */
package com.automatak.render

package object cpp {

  private val quote : String = "\""

  def getEnumType(typ: EnumModel.Type): String = typ match {
    case EnumModel.UInt8 => "uint8_t"
    case EnumModel.UInt16 => "uint16_t"
    case EnumModel.UInt32 => "uint32_t"
  }

  def quoted(s: String): String = List(quote, s, quote).mkString

  def bracketed(s: String): String = List("<", s, ">").mkString

  def concat(a: Iterator[String], b: Iterator[Option[String]]): Iterator[String] = {

    new Iterator[String] {
      def hasNext = a.hasNext && b.hasNext

      def next(): String = {
        val d = a.next()
        b.next() match {
          case Some(c) => d + c
          case None => d
        }
      }
    }
  }

  def stdString: String = "std::string"
  def cString: String = "char const*"

  def namespace(ns: String)(internals: Iterator[String]): Iterator[String] = {

    Iterator.apply(List("namespace",ns,"{").spaced) ++
    space ++
    internals ++
    space ++
    Iterator.apply("}")

  }

  def comment(comment: String)(implicit i: Indentation): Iterator[String] = Iterator("// %s".format(comment))

  def privateSection(inner: => Iterator[String]): Iterator[String] = Iterator("private:") ++ space ++ inner

  def classPublic(inner: => Iterator[String])(implicit i: Indentation): Iterator[String] = Iterator("public:","") ++ inner

  def classPrivate(inner: => Iterator[String])(implicit i: Indentation): Iterator[String] = Iterator("private:","") ++ inner

  def struct(name: String, base: Option[String] = None)(inner: => Iterator[String])(implicit i: Indentation): Iterator[String] = {
    def header = base match {
      case Some(b) => Iterator("struct " + name + " : public " + b)
      case None => Iterator("struct " + name)
    }
    header ++
      bracketSemiColon {
        inner
      }
  }

  def clazz(name: String, bases: List[String] = Nil)(inner: => Iterator[String])(implicit i: Indentation): Iterator[String] = {
    def header = "class " + name
    def headerWithBases = Iterator(if(bases.isEmpty) header else header + ": " + bases.map(b => "public " + b).mkString(","))
    headerWithBases ++
    bracketSemiColon {
      inner
    }
  }

  private def ssp21Pattern(name: String) = "SSP21_"+ name.toUpperCase + "_H"

  def includeGuards(name: String)(internals: => Iterator[String]): Iterator[String] = {

    Iterator("#ifndef %s".format(ssp21Pattern(name))) ++
    Iterator("#define %s".format(ssp21Pattern(name))) ++
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
