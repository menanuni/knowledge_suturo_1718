:- register_ros_package(knowrob_common).

:- use_module(poke_position_calculator,[calculate_poke_position/6]).

:- owl_parser:owl_parse('../owl/suturo_object.owl').