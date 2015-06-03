module Codewars.Kata.DNA where
import Codewars.Kata.DNA.Types

-- data Base = A | T | G | C
type DNA = [Base]

fn A = T
fn T = A
fn C = G
fn G = C
dnaStrand :: DNA -> DNA
dnaStrand = map fn
