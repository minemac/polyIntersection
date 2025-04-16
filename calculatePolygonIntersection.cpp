// Function to calculate the intersection of two polygons using the Sutherland-Hodgman algorithm
std::vector<Point> calculatePolygonIntersection(const std::vector<Point> &subjectPolygon, const std::vector<Point> &clipPolygon)
{
    // Lambda function to check if a point is inside a given edge
    auto isInside = [](const Point &p, const Point &edgeStart, const Point &edgeEnd)
    {
        // Check if the point is on the left side of the edge (or on the edge itself)
        return (edgeEnd.x - edgeStart.x) * (p.y - edgeStart.y) - (edgeEnd.y - edgeStart.y) * (p.x - edgeStart.x) >= 0;
    };

    // Lambda function to compute the intersection point of two lines
    auto computeIntersection = [](const Point &s, const Point &e, const Point &edgeStart, const Point &edgeEnd)
    {
        // Line equation coefficients for the first line (s to e)
        float A1 = e.y - s.y;
        float C1 = s.x - e.x;
        float D1 = A1 * s.x + C1 * s.y;

        // Line equation coefficients for the second line (edgeStart to edgeEnd)
        float A2 = edgeEnd.y - edgeStart.y;
        float C2 = edgeStart.x - edgeEnd.x;
        float D2 = A2 * edgeStart.x + C2 * edgeStart.y;

        // Calculate the determinant to check if the lines are parallel
        float det = A1 * C2 - A2 * C1;
        if (std::abs(det) < 1e-6) // If determinant is close to zero, lines are parallel
        {
            return Point{0, 0}; // Return a default point (no intersection)
        }

        // Calculate the intersection point
        float x = (C2 * D1 - C1 * D2) / det;
        float y = (A1 * D2 - A2 * D1) / det;
        return Point{x, y};
    };

    // Initialize the output list with the subject polygon
    std::vector<Point> outputList = subjectPolygon;

    // Iterate through each edge of the clipping polygon
    for (size_t i = 0; i < clipPolygon.size(); ++i)
    {
        // Define the current edge of the clipping polygon
        Point edgeStart = clipPolygon[i];
        Point edgeEnd = clipPolygon[(i + 1) % clipPolygon.size()];

        // Copy the current output list to the input list
        std::vector<Point> inputList = outputList;
        outputList.clear(); // Clear the output list for the next iteration

        // Iterate through each edge of the subject polygon
        for (size_t j = 0; j < inputList.size(); ++j)
        {
            // Define the current and previous vertices of the subject polygon
            Point current = inputList[j];
            Point prev = inputList[(j + inputList.size() - 1) % inputList.size()];

            // Check if the current vertex is inside the clipping edge
            if (isInside(current, edgeStart, edgeEnd))
            {
                // If the previous vertex is outside, add the intersection point
                if (!isInside(prev, edgeStart, edgeEnd))
                {
                    outputList.push_back(computeIntersection(prev, current, edgeStart, edgeEnd));
                }
                // Add the current vertex to the output list
                outputList.push_back(current);
            }
            // If the current vertex is outside but the previous vertex is inside
            else if (isInside(prev, edgeStart, edgeEnd))
            {
                // Add the intersection point
                outputList.push_back(computeIntersection(prev, current, edgeStart, edgeEnd));
            }
        }
    }

    // Return the resulting polygon after clipping
    return outputList;
}
