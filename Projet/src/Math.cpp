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

bool Math::intersection(const Point& a, const Point& b, const Point& c, const Point& d, Point& intersection) {
    auto divisor = (a.x - b.x) * (c.y - d.y) - (a.y - b.y) * (c.x - d.x);
	if(divisor == 0) return false;

	auto t = (a.x - c.x) * (c.y - d.y) - (a.y - c.y) * (c.x - d.x);
	t /= divisor;
//	auto u = (a.x - b.x) * (a.y - c.y) - (a.y - b.y) * (a.x - c.x);
//	u /= divisor;
	intersection = a + ((b - a) * t);
	return 0 <= t && t <= 1;// && 0 <= u && u <= 1;
}

bool Math::visible(Point S, Point Fi, Point Fi1, Point centroid)
{
	auto n = Fi1 - Fi;
	n = {-n.y, n.x};
	n = Math::Normalize(n);

	auto m = Fi1 - Fi;
	m = {m.y, -m.x};
	m = Math::Normalize(m);

	auto fToC = centroid - Fi;
	fToC = Math::Normalize(fToC);

	auto normal = glm::dot(n, fToC) >= 0 ? n : m;

	return glm::dot(normal, {S - Fi}) >= 0;
}

std::vector<Color> colors = {{173.0f/255.0f,183.0f/255.0f,0.0f/255.0f,1.0f},
							 {0.0f/255.0f,84.0f/255.0f,216.0f/255.0f,1.0f},
							 {150.0f/255.0f,193.0f/255.0f,11.0f/255.0f,1.0f},
							 {118.0f/255.0f,91.0f/255.0f,228.0f/255.0f,1.0f},
							 {118.0f/255.0f,217.0f/255.0f,75.0f/255.0f,1.0f},
							 {160.0f/255.0f,63.0f/255.0f,197.0f/255.0f,1.0f},
							 {45.0f/255.0f,210.0f/255.0f,92.0f/255.0f,1.0f},
							 {194.0f/255.0f,67.0f/255.0f,199.0f/255.0f,1.0f},
							 {58.0f/255.0f,171.0f/255.0f,31.0f/255.0f,1.0f},
							 {222.0f/255.0f,104.0f/255.0f,238.0f/255.0f,1.0f},
							 {127.0f/255.0f,194.0f/255.0f,32.0f/255.0f,1.0f},
							 {137.0f/255.0f,28.0f/255.0f,155.0f/255.0f,1.0f},
							 {1.0f/255.0f,172.0f/255.0f,55.0f/255.0f,1.0f},
							 {183.0f/255.0f,0.0f/255.0f,151.0f/255.0f,1.0f},
							 {90.0f/255.0f,168.0f/255.0f,0.0f/255.0f,1.0f},
							 {245.0f/255.0f,83.0f/255.0f,205.0f/255.0f,1.0f},
							 {0.0f/255.0f,141.0f/255.0f,28.0f/255.0f,1.0f},
							 {195.0f/255.0f,127.0f/255.0f,255.0f/255.0f,1.0f},
							 {216.0f/255.0f,195.0f/255.0f,0.0f/255.0f,1.0f},
							 {0.0f/255.0f,99.0f/255.0f,211.0f/255.0f,1.0f},
							 {253.0f/255.0f,170.0f/255.0f,20.0f/255.0f,1.0f},
							 {1.0f/255.0f,145.0f/255.0f,254.0f/255.0f,1.0f},
							 {240.0f/255.0f,118.0f/255.0f,15.0f/255.0f,1.0f},
							 {1.0f/255.0f,113.0f/255.0f,219.0f/255.0f,1.0f},
							 {203.0f/255.0f,205.0f/255.0f,59.0f/255.0f,1.0f},
							 {83.0f/255.0f,66.0f/255.0f,167.0f/255.0f,1.0f},
							 {90.0f/255.0f,223.0f/255.0f,122.0f/255.0f,1.0f},
							 {222.0f/255.0f,0.0f/255.0f,118.0f/255.0f,1.0f},
							 {14.0f/255.0f,224.0f/255.0f,171.0f/255.0f,1.0f},
							 {249.0f/255.0f,43.0f/255.0f,92.0f/255.0f,1.0f},
							 {87.0f/255.0f,222.0f/255.0f,145.0f/255.0f,1.0f},
							 {173.0f/255.0f,0.0f/255.0f,117.0f/255.0f,1.0f},
							 {80.0f/255.0f,142.0f/255.0f,0.0f/255.0f,1.0f},
							 {134.0f/255.0f,137.0f/255.0f,255.0f/255.0f,1.0f},
							 {51.0f/255.0f,125.0f/255.0f,0.0f/255.0f,1.0f},
							 {255.0f/255.0f,84.0f/255.0f,169.0f/255.0f,1.0f},
							 {0.0f/255.0f,142.0f/255.0f,66.0f/255.0f,1.0f},
							 {219.0f/255.0f,1.0f/255.0f,83.0f/255.0f,1.0f},
							 {0.0f/255.0f,200.0f/255.0f,151.0f/255.0f,1.0f},
							 {196.0f/255.0f,0.0f/255.0f,43.0f/255.0f,1.0f},
							 {0.0f/255.0f,192.0f/255.0f,172.0f/255.0f,1.0f},
							 {239.0f/255.0f,58.0f/255.0f,61.0f/255.0f,1.0f},
							 {1.0f/255.0f,172.0f/255.0f,117.0f/255.0f,1.0f},
							 {183.0f/255.0f,0.0f/255.0f,103.0f/255.0f,1.0f},
							 {172.0f/255.0f,212.0f/255.0f,89.0f/255.0f,1.0f},
							 {50.0f/255.0f,74.0f/255.0f,171.0f/255.0f,1.0f},
							 {183.0f/255.0f,155.0f/255.0f,0.0f/255.0f,1.0f},
							 {0.0f/255.0f,117.0f/255.0f,209.0f/255.0f,1.0f},
							 {255.0f/255.0f,159.0f/255.0f,57.0f/255.0f,1.0f},
							 {1.0f/255.0f,155.0f/255.0f,237.0f/255.0f,1.0f},
							 {250.0f/255.0f,99.0f/255.0f,45.0f/255.0f,1.0f},
							 {1.0f/255.0f,146.0f/255.0f,212.0f/255.0f,1.0f},
							 {210.0f/255.0f,127.0f/255.0f,0.0f/255.0f,1.0f},
							 {1.0f/255.0f,93.0f/255.0f,167.0f/255.0f,1.0f},
							 {210.0f/255.0f,202.0f/255.0f,81.0f/255.0f,1.0f},
							 {91.0f/255.0f,69.0f/255.0f,147.0f/255.0f,1.0f},
							 {160.0f/255.0f,213.0f/255.0f,120.0f/255.0f,1.0f},
							 {133.0f/255.0f,48.0f/255.0f,123.0f/255.0f,1.0f},
							 {102.0f/255.0f,219.0f/255.0f,168.0f/255.0f,1.0f},
							 {177.0f/255.0f,0.0f/255.0f,68.0f/255.0f,1.0f},
							 {0.0f/255.0f,122.0f/255.0f,69.0f/255.0f,1.0f},
							 {255.0f/255.0f,157.0f/255.0f,252.0f/255.0f,1.0f},
							 {29.0f/255.0f,96.0f/255.0f,9.0f/255.0f,1.0f},
							 {255.0f/255.0f,127.0f/255.0f,200.0f/255.0f,1.0f},
							 {77.0f/255.0f,100.0f/255.0f,0.0f/255.0f,1.0f},
							 {205.0f/255.0f,162.0f/255.0f,255.0f/255.0f,1.0f},
							 {182.0f/255.0f,98.0f/255.0f,0.0f/255.0f,1.0f},
							 {148.0f/255.0f,163.0f/255.0f,255.0f/255.0f,1.0f},
							 {155.0f/255.0f,41.0f/255.0f,2.0f/255.0f,1.0f},
							 {0.0f/255.0f,156.0f/255.0f,204.0f/255.0f,1.0f},
							 {255.0f/255.0f,98.0f/255.0f,73.0f/255.0f,1.0f},
							 {1.0f/255.0f,154.0f/255.0f,138.0f/255.0f,1.0f},
							 {255.0f/255.0f,87.0f/255.0f,130.0f/255.0f,1.0f},
							 {48.0f/255.0f,93.0f/255.0f,51.0f/255.0f,1.0f},
							 {255.0f/255.0f,151.0f/255.0f,198.0f/255.0f,1.0f},
							 {106.0f/255.0f,96.0f/255.0f,0.0f/255.0f,1.0f},
							 {166.0f/255.0f,181.0f/255.0f,255.0f/255.0f,1.0f},
							 {127.0f/255.0f,86.0f/255.0f,0.0f/255.0f,1.0f},
							 {200.0f/255.0f,189.0f/255.0f,255.0f/255.0f,1.0f},
							 {88.0f/255.0f,85.0f/255.0f,12.0f/255.0f,1.0f},
							 {0.0f/255.0f,111.0f/255.0f,169.0f/255.0f,1.0f},
							 {255.0f/255.0f,134.0f/255.0f,83.0f/255.0f,1.0f},
							 {87.0f/255.0f,76.0f/255.0f,125.0f/255.0f,1.0f},
							 {246.0f/255.0f,188.0f/255.0f,99.0f/255.0f,1.0f},
							 {127.0f/255.0f,57.0f/255.0f,111.0f/255.0f,1.0f},
							 {204.0f/255.0f,202.0f/255.0f,116.0f/255.0f,1.0f},
							 {143.0f/255.0f,48.0f/255.0f,78.0f/255.0f,1.0f},
							 {165.0f/255.0f,172.0f/255.0f,116.0f/255.0f,1.0f},
							 {255.0f/255.0f,124.0f/255.0f,141.0f/255.0f,1.0f},
							 {132.0f/255.0f,63.0f/255.0f,16.0f/255.0f,1.0f},
							 {219.0f/255.0f,164.0f/255.0f,207.0f/255.0f,1.0f},
							 {149.0f/255.0f,47.0f/255.0f,44.0f/255.0f,1.0f},
							 {234.0f/255.0f,190.0f/255.0f,143.0f/255.0f,1.0f},
							 {158.0f/255.0f,124.0f/255.0f,172.0f/255.0f,1.0f},
							 {255.0f/255.0f,174.0f/255.0f,118.0f/255.0f,1.0f},
							 {160.0f/255.0f,107.0f/255.0f,146.0f/255.0f,1.0f},
							 {255.0f/255.0f,167.0f/255.0f,145.0f/255.0f,1.0f},
							 {127.0f/255.0f,65.0f/255.0f,51.0f/255.0f,1.0f},
							 {255.0f/255.0f,166.0f/255.0f,178.0f/255.0f,1.0f},
							 {139.0f/255.0f,99.0f/255.0f,61.0f/255.0f,1.0f}};


std::vector<Point> Math::sutherlandHodgman(std::vector<Point> PL, std::vector<Point> PW, Application& app) {
	Point centroid = {0,0};
	for (const auto& i : PW) {
		centroid += i;
	}
	centroid /= PW.size();

    std::vector<Point> result = {};
	int colorUsed = 0;

    for (int i = 0; i < PW.size()-1; ++i) {
        std::vector<Point> PS;

        Point S, F, I;

        for (int j = 0; j < PL.size(); ++j) {
            if (j == 0) {
                F = PL[j];
            } else {
				if(intersection(S, PL[j], PW[i], PW[i + 1], I))
				{
					app.DrawWorldPoint(I, 2, colors[colorUsed++]);
                    PS.push_back(I);
                }
            }

            S = PL[j];

            if (visible(S, PW[i], PW[i + 1], centroid)) {
                PS.push_back(S);
				app.DrawWorldPoint(S, 2, colors[colorUsed++]);
            }
        }

        if (!PS.empty()) {
            if (intersection(S, F, PW[i], PW[i + 1], I)) {
                PS.push_back(I);
				app.DrawWorldPoint(I, 2, colors[colorUsed++]);
            }
			PL = PS;
        }
    }

    return PL;
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
	std::vector<Vec2Int> positionsToVisit = {{x,y}};

	while(!positionsToVisit.empty())
	{
        Vec2Int position = positionsToVisit[positionsToVisit.size()-1];
        positionsToVisit.pop_back();
        if (position.x < min.x || position.x >= max.x || position.y < min.y || position.y >= max.y)
        {continue;}

        if (shouldBeFilled(position.x, position.y, app, colorContour, colorFill))
        {
            app.WriteWorldPixel(position, colorFill);

            positionsToVisit.push_back(Vec2Int(position.x +0, position.y -1));
            positionsToVisit.push_back(Vec2Int(position.x -1, position.y +0));
            positionsToVisit.push_back(Vec2Int(position.x +0, position.y +1));
            positionsToVisit.push_back(Vec2Int(position.x +1, position.y +0));
        }
	}
}

void Math::fillRegionConnexity4Recursive(int x, int y, Vec2Int min, Vec2Int max, Application& app, const Color& colorContour, const Color& colorFill)
{
    // Vérifier si les coordonnées sont dans les limites de l'image
    if (x < min.x || x >= max.x || y < min.y || y >= max.y) return;

    // Vérifier si la couleur actuelle n'est ni la couleur de contour ni celle de remplissage
    auto color = app.ReadWorldPixel({x,y});
    if (shouldBeFilled(x, y, app, colorContour, colorFill)) {
        app.WriteWorldPixel({x,y}, colorFill);

        // Appels récursifs pour remplir les régions adjacentes
        fillRegionConnexity4(x + 0, y - 1, min, max, app, colorContour, colorFill); // Bas
        fillRegionConnexity4(x - 1, y + 0, min, max, app, colorContour, colorFill); // Gauche
        fillRegionConnexity4(x + 0, y + 1, min, max, app, colorContour, colorFill); // Haut
        fillRegionConnexity4(x + 1, y + 0, min, max, app, colorContour, colorFill); // Droite
    }
}

//remplissage ligne par ligne
void Math::fillRegionLineByLine(int x, int y, Vec2Int min, Vec2Int max, Application& app, const Color& colorContour, const Color& colorFill)
{
    std::vector<Vec2Int> positionsToVisit = {{x,y}};

    while(!positionsToVisit.empty())
    {
        Vec2Int position = positionsToVisit[positionsToVisit.size()-1];
        positionsToVisit.pop_back();

        if (position.x < min.x || position.x >= max.x || position.y < min.y || position.y >= max.y)
        {continue;}

        if (!shouldBeFilled(position.x, position.y, app, colorContour, colorFill))
            continue;

        int left = position.x;
        while (left > min.x && shouldBeFilled(left - 1, position.y, app, colorContour, colorFill)) left--;

        int right = position.x;
        while (right < max.x && shouldBeFilled(right + 1, position.y, app, colorContour, colorFill)) right++;

        // Remplir la ligne et préparer la visite des lignes supérieures et inférieures
        for (int i = left; i <= right; i++) {
            app.WriteWorldPixel(Vec2Int(i, position.y), colorFill);

            if (position.y > min.y && shouldBeFilled(i, position.y - 1, app, colorContour, colorFill))
                positionsToVisit.push_back(Vec2Int(i, position.y - 1));
            if (position.y < max.y - 1 && shouldBeFilled(i, position.y + 1, app, colorContour, colorFill))
                positionsToVisit.push_back(Vec2Int(i, position.y + 1));
        }

    }
}

//remplissage de bubulleee  
void Math::fillRegion(int x, int y, std::function<bool(int,int)> boundChecker, Application& app, const Color& colorContour, const Color& colorFill)
{
    std::vector<Vec2Int> positionsToVisit = {{x,y}};

    while(!positionsToVisit.empty()) {
        Vec2Int position = positionsToVisit.back();
        positionsToVisit.pop_back();

        // Utilisation de boundChecker pour vérifier les limites et shouldBeFilled pour la logique de remplissage
        if (!boundChecker(position.x, position.y) || !shouldBeFilled(position.x, position.y, app, colorContour, colorFill)) {
            continue;
        }

        // Remplir le pixel avec la couleur de remplissage
        app.WriteWorldPixel(position, colorFill);

        // Ajouter les pixels voisins à la liste des positions à visiter
        positionsToVisit.push_back(Vec2Int(position.x + 1, position.y)); // Droite
        positionsToVisit.push_back(Vec2Int(position.x - 1, position.y)); // Gauche
        positionsToVisit.push_back(Vec2Int(position.x, position.y + 1)); // Bas
        positionsToVisit.push_back(Vec2Int(position.x, position.y - 1)); // Haut
    }
}


bool Math::shouldBeFilled(int x, int y, Application& app, const Color& colorContour, const Color& colorFill) {
    auto color = app.ReadWorldPixel(Vec2Int(x, y));
    glm::vec<4, bool> isFill = glm::epsilonEqual(color, colorFill, Vec4(0.01f));
    glm::vec<4, bool> isContour = glm::epsilonEqual(color, colorContour, Vec4(0.01f));
    return !(isFill.x && isFill.y && isFill.z && isFill.w) && !(isContour.x && isContour.y && isContour.z && isContour.w);
}