import pandas as pd
import requests
import math

# Haversine formula to calculate distance between two points on the Earth
def haversine(lat1, lon1, lat2, lon2):
    R = 6371  # Radius of the Earth in km
    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)
    a = math.sin(dlat / 2) ** 2 + math.cos(math.radians(lat1)) * math.cos(math.radians(lat2)) * math.sin(dlon / 2) ** 2
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c

# Fetch and parse routes data from OpenFlights
def fetch_routes_data():
    routes_url = "https://raw.githubusercontent.com/jpatokal/openflights/master/data/routes.dat"
    routes_df = pd.read_csv(routes_url, header=None, names=["Airline", "Airline ID", "Source Airport", "Source Airport ID", "Destination Airport", "Destination Airport ID", "Codeshare", "Stops", "Equipment"])
    return routes_df

# Fetch and parse airport data from OpenFlights
def fetch_airport_data():
    airports_url = "https://raw.githubusercontent.com/jpatokal/openflights/master/data/airports.dat"
    airports_df = pd.read_csv(airports_url, header=None, names=["Airport ID", "Name", "City", "Country", "IATA", "ICAO", "Latitude", "Longitude", "Altitude", "Timezone", "DST", "Tz database timezone", "Type", "Source"])
    return airports_df

# Main function to handle the process
def main():
    # Fetch data
    airports_df = fetch_airport_data()
    routes_df = fetch_routes_data()
    
    # Merge route data with airport data to get coordinates
    merged_routes = routes_df.merge(airports_df[['IATA', 'Latitude', 'Longitude']], left_on='Source Airport', right_on='IATA', suffixes=('', '_source'))
    merged_routes = merged_routes.merge(airports_df[['IATA', 'Latitude', 'Longitude']], left_on='Destination Airport', right_on='IATA', suffixes=('', '_destination'))

    # Compute distances
    merged_routes['Distance'] = merged_routes.apply(lambda row: haversine(row['Latitude'], row['Longitude'], row['Latitude_destination'], row['Longitude_destination']), axis=1)
    
    # Save to CSV
    merged_routes.to_csv('routes_with_distances.csv', index=False)
    print("Routes with distances saved to 'routes_with_distances.csv'")

if __name__ == "__main__":
    main()
