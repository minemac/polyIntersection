std::vector<Point> calculatePolygonIntersection(const std::vector<Point> &subjectPolygon, const std::vector<Point> &clipPolygon)
{
    auto isInside = [](const Point &p, const Point &edgeStart, const Point &edgeEnd)
    {
        return (edgeEnd.x - edgeStart.x) * (p.y - edgeStart.y) - (edgeEnd.y - edgeStart.y) * (p.x - edgeStart.x) >= 0;
    };

    auto computeIntersection = [](const Point &s, const Point &e, const Point &edgeStart, const Point &edgeEnd)
    {
        float A1 = e.y - s.y;
        float C1 = s.x - e.x;
        float D1 = A1 * s.x + C1 * s.y;

        float A2 = edgeEnd.y - edgeStart.y;
        float C2 = edgeStart.x - edgeEnd.x;
        float D2 = A2 * edgeStart.x + C2 * edgeStart.y;

        float det = A1 * C2 - A2 * C1;
        if (std::abs(det) < 1e-6)
        {
            return Point{0, 0}; // Paralelní linie
        }

        float x = (C2 * D1 - C1 * D2) / det;
        float y = (A1 * D2 - A2 * D1) / det;
        return Point{x, y};
    };

    std::vector<Point> outputList = subjectPolygon;

    for (size_t i = 0; i < clipPolygon.size(); ++i)
    {
        Point edgeStart = clipPolygon[i];
        Point edgeEnd = clipPolygon[(i + 1) % clipPolygon.size()];

        std::vector<Point> inputList = outputList;
        outputList.clear();

        for (size_t j = 0; j < inputList.size(); ++j)
        {
            Point current = inputList[j];
            Point prev = inputList[(j + inputList.size() - 1) % inputList.size()];

            if (isInside(current, edgeStart, edgeEnd))
            {
                if (!isInside(prev, edgeStart, edgeEnd))
                {
                    outputList.push_back(computeIntersection(prev, current, edgeStart, edgeEnd));
                }
                outputList.push_back(current);
            }
            else if (isInside(prev, edgeStart, edgeEnd))
            {
                outputList.push_back(computeIntersection(prev, current, edgeStart, edgeEnd));
            }
        }
    }

    return outputList;
}
