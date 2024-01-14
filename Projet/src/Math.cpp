//
// Created by ianpo on 29/12/2023.
//

#include "Math.hpp"
#include "Application.hpp"

Mat4 Math::TRS(Vec2 position)
{
	return TRS(position, Vec2(1.0f, 1.0f));
}

Mat4 Math::TRS(Vec2 position, Vec2 scale)
{
	return TRS(Vec3(position.x, position.y, 0), glm::identity<glm::quat>(), Vec3(scale.x, scale.y, 0));
}

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

double Math:: coupe(Point S, Point Pj, Point Fi, Point Fi1) {
    double crossProduct = (Pj.x - S.x) * (Fi1.y - Fi.y) - (Pj.y - S.y) * (Fi1.x - Fi.x);
    return crossProduct < 0;
}


Point Math:: intersection(const Point& a, const Point& b, const Point& c, const Point& d) {
    double ua = ((d.x - c.x) * (a.y - c.y) - (d.y - c.y) * (a.x - c.x)) /
                ((d.y - c.y) * (b.x - a.x) - (d.x - c.x) * (b.y - a.y));
    Point intersection_point = {a.x + ua * (b.x - a.x), a.y + ua * (b.y - a.y)};
    return intersection_point;
}

bool Math::visible(Point S, Point Fi, Point Fi1){
    double crossProduct = (Fi1.x - Fi.x) * (S.y - Fi.y) - (Fi1.y - Fi.y) * (S.x - Fi.x);
    return crossProduct >= 0;
}

std::vector<Point> Math::sutherlandHodgman(std::vector<Point> PL, std::vector<Point> PW) {
    std::vector<Point> result;

    for (int i = 0; i < PW.size(); ++i) {
        int N2 = 0;
        std::vector<Point> PS;

        Point S, F, I;

        for (int j = 0; j < PL.size(); ++j) {
            if (j == 0) {
                F = PL.back();
            } else {
                if (coupe(S, PL[j], PW[i], PW[(i + 1) % PW.size()])) {
                    I = intersection(S, PL[j], PW[i], PW[(i + 1) % PW.size()]);
                    PS.push_back(I);
                    N2++;
                }
            }

            S = PL[j];

            if (visible(S, PW[i], PW[(i + 1) % PW.size()])) {
                PS.push_back(S);
                N2++;
            }
        }

        if (N2 > 0) {
            if (coupe(S, F, PW[i], PW[(i + 1) % PW.size()])) {
                I = intersection(S, F, PW[i], PW[(i + 1) % PW.size()]);
                PS.push_back(I);
                N2++;
            }
        }


        result.insert(result.end(), PS.begin(), PS.end());
    }

    return result;
}

/*
struct Point {
    int x, y;
};*/

struct Edge {
    int ymin, ymax;
    float x, dx;
    // Constructeur pour initialiser une arête
    Edge(int ymn, int ymx, float xPos, float slope) : ymin(ymn), ymax(ymx), x(xPos), dx(slope) {}
};

// Crée une liste d'arêtes à partir des points du polygone
std::vector<Edge> create_edges(const std::vector<Point>& poly) {
    std::vector<Edge> edges;
    size_t N = poly.size();
    for (size_t i = 0; i < N; ++i) {
        Point p1 = poly[i];
        Point p2 = poly[(i + 1) % N];
        if (p1.y != p2.y) {
            int ymin = std::min(p1.y, p2.y);
            int ymax = std::max(p1.y, p2.y);
            float x = (p1.y == ymin) ? p1.x : p2.x;
            float dx = static_cast<float>(p2.x - p1.x) / (p2.y - p1.y);
            edges.emplace_back(ymin, ymax, x, dx);
        }
    }
    return edges;
}

// Met à jour la table des arêtes actives (LCA) pour la ligne de balayage y
void update_active_edge_table(std::vector<Edge>& aet, const std::vector<Edge>& edges, int y) {
    aet.erase(std::remove_if(aet.begin(), aet.end(), [y](const Edge& edge) { return edge.ymax <= y; }), aet.end());
    for (const auto& edge : edges) {
        if (edge.ymin == y) {
            aet.push_back(edge);
        }
    }
}

// Remplit l'espace entre deux arêtes actives dans l'image
void fill_between_edges(const Edge& edge1, const Edge& edge2, int y, Application& app, const Color& color_fill) {
    int x_start = static_cast<int>(edge1.x);
    int x_end = static_cast<int>(edge2.x);
    for (int x = x_start; x < x_end; ++x) {
		app.WriteWorldPixel({x,y}, color_fill);
    }
}

// Effectue le remplissage de polygone en utilisant l'algorithme de balayage
void Math::polygon_fill(const std::vector<Point>& poly, Application& app, const Color& color_fill) {
    std::vector<Edge> edges = create_edges(poly);
    int min_y = std::min_element(poly.begin(), poly.end(), [](const Point& a, const Point& b) { return a.y < b.y; })->y;
    int max_y = std::max_element(poly.begin(), poly.end(), [](const Point& a, const Point& b) { return a.y < b.y; })->y;

    std::vector<Edge> active_edge_table;
    for (int y = min_y; y <= max_y; ++y) {
        update_active_edge_table(active_edge_table, edges, y);
        std::sort(active_edge_table.begin(), active_edge_table.end(), [](const Edge& a, const Edge& b) { return a.x < b.x; });

        for (size_t i = 0; i < active_edge_table.size(); i += 2) {
            if (i + 1 < active_edge_table.size()) {
                fill_between_edges(active_edge_table[i], active_edge_table[i + 1], y, app, color_fill);
            }
        }

        for (Edge& edge : active_edge_table) {
            edge.x += edge.dx;
        }
    }
}

// Effectue le remplissage par region
void Math::fillRegionConnexity4(int x, int y, Vec2Int min, Vec2Int max, Application& app, const Color& colorContour, const Color& colorFill) {
    // Vérifier si les coordonnées sont dans les limites de l'image
    if (x < min.x || x >= max.x || y < min.y || y >= max.y) return;

    // Vérifier si la couleur actuelle n'est ni la couleur de contour ni celle de remplissage
	auto color = app.ReadWorldPixel({x,y});
    if (color != colorContour && color != colorFill) {
		app.WriteWorldPixel({x,y}, colorFill);

        // Appels récursifs pour remplir les régions adjacentes
        fillRegionConnexity4(x + 0, y - 1, min, max, app, colorContour, colorFill); // Bas
        fillRegionConnexity4(x - 1, y + 0, min, max, app, colorContour, colorFill); // Gauche
        fillRegionConnexity4(x + 0, y + 1, min, max, app, colorContour, colorFill); // Haut
        fillRegionConnexity4(x + 1, y + 0, min, max, app, colorContour, colorFill); // Droite
    }
}