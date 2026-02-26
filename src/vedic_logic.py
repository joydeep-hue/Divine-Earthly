
def vedic_scaling_prepro(weights):
    # O(1) transformation for specific bit-depths
    transformed_weights = []
    for w in weights:
        # Applying Ekadhikena logic for fast square-root approximations
        # in the Euclidean distance calculation of the grid nodes
        a = w // 10
        transformed_weights.append((a * (a + 1)) * 100 + 25)
    return transformed_weights
