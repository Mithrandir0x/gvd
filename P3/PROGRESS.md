# Abstract

Per seguir aquest informe de pràctica s'ha de tenir en compte que s'ha partit
des de la implementació existent de la segona pràctica.

---

# Correccions sobre la pràctica 2

Aquesta primera part es dedicarà a descriure els canvis introduïts fets arrel
de les observacions donades per la segona pràctica.

- Hem interpretat que amb la càmera en primera persona és de la bola, i que al
  premer les tecles per moure cap a l'esquerra o la dreta feia que moves la bola
  i no l'angle de la càmera com si fos el pal. El dia del test, la profesora ens
  va comentar que s'havia de canviar aquestes coses i només vam poguer arreglar
  el fet que no es veia la bola (el canvi és que ara és la càmera en primera
  persona del pal, en comptes de ser la de la bola).

- La implementació presentada de la segona extensió s'ha desactivat, degut a
  possibles problemes de rendiment quan es transicionava de càmera general a
  càmera en primera persona. No obstant, el codi està implementat a `GLWidget::keyPressEvent`.

---

# Progrés de la pràctica
