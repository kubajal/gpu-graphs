
import ast
from collections import defaultdict

ATTR_UNSIGNED = 0

def ast_to_dict(node):
    if isinstance(node, ast.AST):
        node_dict = {"type": type(node).__name__}
        # For fields that are nodes, lists of nodes, or leaf values
        for field, value in ast.iter_fields(node):
            if isinstance(value, ast.AST):
                # Recurse for single nodes
                node_dict[field] = ast_to_dict(value)
            elif isinstance(value, list):
                # Recurse for each item in lists
                node_dict[field] = [
                    ast_to_dict(item) if isinstance(item, ast.AST) else item
                    for item in value
                ]
            else:
                # Directly assign leaf values
                node_dict[field] = value
        return node_dict
    else:
        # Non-AST objects (should not occur, but included for completeness)
        return repr(node)

def load_python_file(file_name):
    # Your Python code
    with open(file_name) as file:
        code = file.read()
        tree = ast.parse(code)
        ast_dict = ast_to_dict(tree)
        return ast_dict
    raise Exception("Should not have reached this code")

def ast_dict_to_incidence_list_and_attributes(ast_dict):
    """
    Convert an AST dictionary to an incidence dict and attributes dict.
    Parameters:
    ast_dict (dict[str,str]): The AST dictionary.
    Returns:
    tuple: A tuple containing the incidence dict and attributes dict.
    """
    node_counter = 0
    attribute_counter = 0
    edges: dict[int, list[int]] = defaultdict(list[int])
    nodes: dict[int, int] = {}
    attributes_lookup: dict[int, str] = {}

    def get_attribute_id(attribute):
        nonlocal attribute_counter
        nonlocal attributes_lookup
        if attribute in attributes_lookup:
            return attributes_lookup[attribute]
        attributes_lookup[attribute] = attribute_counter
        attribute_counter = attribute_counter + 1
        return attributes_lookup[attribute]

    def process_node(node, current_node=None):
        nonlocal node_counter
        nonlocal edges
        nonlocal nodes
        if current_node == None:
            current_node = 0
            nodes[current_node] = (ATTR_UNSIGNED, get_attribute_id("_root"))
        if isinstance(node, dict):
            # If the node is a rule (has children), process its children
            for key, value in node.items():
                node_counter = node_counter + 1
                edges[current_node].append((ATTR_UNSIGNED, node_counter))
                nodes[node_counter] = (ATTR_UNSIGNED, get_attribute_id(key))
                process_node(value, node_counter)
        elif isinstance(node, str):
            # Leaf node, so no children to process
            node_counter = node_counter + 1
            edges[current_node].append((ATTR_UNSIGNED, node_counter))
            nodes[node_counter] = (ATTR_UNSIGNED, get_attribute_id(node))
        elif isinstance(node, list):
            # Children of a rule
            for child in node:
                process_node(child, node_counter)
        elif isinstance(node, object):
            # Any other type
            node_counter = node_counter + 1
            edges[current_node].append((ATTR_UNSIGNED, node_counter))
            nodes[node_counter] = (ATTR_UNSIGNED, get_attribute_id(f"_{node}"))
        else:
            raise Exception("Should not have reached this code")

    # Start processing from the root node
    process_node(ast_dict)
    return edges, nodes, attributes_lookup

def ast_to_tree(file_name):
    ast_dict = load_python_file(file_name)
    return ast_dict_to_incidence_list_and_attributes(ast_dict)
