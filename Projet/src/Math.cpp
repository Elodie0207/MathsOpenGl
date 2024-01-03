//
// Created by ianpo on 29/12/2023.
//

#include "Math.hpp"

Mat4 Math::TRS(Vec3 position)
{
	return TRS(position, glm::identity<glm::quat>(), Vec3(1.0f, 1.0f, 1.0f));
}

Mat4 Math::TRS(Vec3 position, Quat rotation)
{
	return TRS(position, rotation, Vec3(1.0f, 1.0f, 1.0f));
}

Mat4 Math::TRS(Vec3 position, Vec3 scale)
{
	return TRS(position, glm::identity<glm::quat>(), scale);
}

Mat4 Math::TRS(Vec3 position, Quat rotation, Vec3 scale) {
	glm::mat4 trs(1.0f);
	trs = glm::translate(trs, position); // Translation Matrix
	trs = trs * glm::toMat4(rotation); // Rotation Matrix
	trs = glm::scale(trs, scale); // Scale Matrix
	return trs; // Translation * Rotation * Scale => TRS Matrix.
}

Mat4 Math::Inverse(Mat4 mat)
{
	return glm::inverse(mat);
}

bool Math::CyrusBeck(double X1, double Y1, double X2, double Y2, std::vector<Point> Poly, std::vector<Point> Normale, int Nbsom)
{
	double t, tinf = std::numeric_limits<double>::lowest(), tsup = std::numeric_limits<double>::max();
	double DX = X2 - X1, DY = Y2 - Y1;
	int Nbseg = Nbsom - 1;

	for (int i = 0; i < Nbseg; ++i) {
		Point C = Poly[i];
		double DN = DX * Normale[i].x + DY * Normale[i].y;
		double WN = (X1 - C.x) * Normale[i].x + (Y1 - C.y) * Normale[i].y;

		if (DN == 0) {
			return (WN > 0);
		} else {
			t = -(WN) / (DN);
			if (DN > 0) {
				if (t > tinf) {
					tinf = t;
				}
			} else {
				if (t < tsup) {
					tsup = t;
				}
			}
		}
	}

	if (tinf < tsup) {
		if ((tinf < 0) && (tsup > 1)) {
			return true;
		} else {
			if ((tinf > 1) || (tsup < 0)) {
				return false;
			} else {
				if (tinf < 0) {
					tinf = 0;
				} else if (tsup > 1) {
					tsup = 1;
				}
				X2 = X1 + DX * tsup;
				Y2 = Y1 + DY * tsup;
				X1 = X1 + DX * tinf;
				Y1 = Y1 + DY * tinf;
				return true;
			}
		}
	} else {
		return false;
	}
}

double Math:: coupe(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}


Point Math:: intersection(const Point& a, const Point& b, const Point& c, const Point& d) {
    double ua = ((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x)) /
                ((d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y));
    Point intersection_point = {a.x + ua * (b.x - a.x), a.y + ua * (b.y - a.y)};
    return intersection_point;
}


std::vector<Point> Math::sutherlandHodgman(std::vector<Point> PL, const std::vector<Point>& PW) {
    std::vector<Point> PS;
    for (size_t i = 0; i < PW.size() - 1; ++i) {
        size_t N2 = 0;
        PS.clear();
        Point S, F, I;
        for (size_t j = 0; j < PL.size(); ++j) {
            if (j == 0) {
                F = PL[j];
            } else {
                if (coupe(S, PL[j], PW[i]) >= 0) {
                    if (coupe(S, PL[j], PW[i + 1]) < 0) {
                        I = intersection(S, PL[j], PW[i], PW[i + 1]);
                        PS.push_back(I);
                        ++N2;
                    }
                } else {
                    if (coupe(S, PL[j], PW[i + 1]) >= 0) {
                        I = intersection(S, PL[j], PW[i], PW[i + 1]);
                        PS.push_back(I);
                        ++N2;
                    }
                }
            }
            S = PL[j];
            if (coupe(S, PW[i], PW[i + 1]) >= 0) {
                PS.push_back(S);
                ++N2;
            }
        }
        if (N2 > 0) {
            if (coupe(S, F, PW[i]) >= 0) {
                if (coupe(S, F, PW[i + 1]) < 0) {
                    I = intersection(S, F, PW[i], PW[i + 1]);
                    PS.push_back(I);
                    ++N2;
                }
            } else {
                if (coupe(S, F, PW[i + 1]) >= 0) {
                    I = intersection(S, F, PW[i], PW[i + 1]);
                    PS.push_back(I);
                    ++N2;
                }
            }
            PL.clear();
            PL = PS;

        }
    }
    return PS;
}