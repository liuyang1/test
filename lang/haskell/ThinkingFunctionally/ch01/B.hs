-- 哪个式子是sin^2(theta)的表示

toRadius x = x / 180 * pi
toDegree x = x * 180 / pi

round2pi x
  | x < 0 = round2pi (x + phi)
  | x > phi = round2pi (x - phi)
  | otherwise = x
  where phi = 2 * pi

-- f0 theta = sin^2 theta -- invalid in Haskell

f1 theta = sin theta ^ 2

-- f2 theta = (sin theta) ^ 2
-- redundant bracket in Haskell as function calling is hightest priority.
-- hlint could find this issue.

g theta = sin (2 * theta) / (2 * pi)
